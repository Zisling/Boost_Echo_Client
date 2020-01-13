

#include <../include/Books.h>
#include <../include/UserIO.h>
#include <boost/thread.hpp>
#include <utility>
#include <../include/SocketIO.h>

SocketIO::SocketIO(std::string userName, ConnectionHandler &connectionHandler, Books &library,boost::atomic_bool *connected)
:userName_(std::move(userName)), connectionHandler(connectionHandler),library(library), connected_(connected) {}


SocketIO::SocketIO(const SocketIO & otherSocketIO)
:userName_(otherSocketIO.userName_),connectionHandler(otherSocketIO.connectionHandler),library(otherSocketIO.library),connected_(otherSocketIO.connected_) {
}

SocketIO &SocketIO::operator=(const SocketIO & otherSocketIO) {
    userName_= otherSocketIO.userName_;

    return <#initializer#>;
}

void SocketIO::run() {

    std::string answer;//server's answer

    while (connected_->load())
    {

        answer.clear();

        //if getting frame fails,disconnects.
        if (!connectionHandler.getFrameAscii(answer,'\0'))
        {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }


        answer.resize(answer.length()-1);


        std::cout<<answer<<std::endl;

        if (answer.find("MESSAGE")!=std::string::npos)
        {
            MessageProcess(answer);
        }

        else if(answer.find("RECEIPT")!=std::string::npos)
        {
            std::string res = "receipt-id:";
            int pos = answer.find(res);
            std::string id = answer.substr(pos+res.length(),answer.find('\n',pos)-pos-res.length());
            std::string action = library.getReceipt(id);
            if (action!="error receipt don't exist")
            {
            if (action.find("DISCONNECT")!=std::string::npos)
            {
                std::cout<<"Disconnecting!"<<std::endl;
                connected_->store(false);
                break;
            }
            else if (action.find("Joined club ")!=std::string::npos)
            {
                int pos1 = action.find("club ")+5;
                int end = action.find('\n');
                std::string genre=action.substr(pos1,end-pos1);
                int pos2=action.find_last_of(':')+1;
                std::string idOfGen =action.substr(pos2);
                library.addId(genre,idOfGen);
                std::cout<<action.substr(0,action.find('\n'))<<std::endl;
            }
            else if (action.find("Exited club ")!=std::string::npos)
            {
                int pos = action.find_last_of("club ")+5;
                std::string genre = action.substr(pos);
                library.removeId(genre);
                std::cout<<action<<std::endl;
            }
            }
            else
                {
                std::cout<<"error receipt don't exist"<<std::endl;
            }
        }

        else if (answer.find("ERROR")!=std::string::npos)
        {
            connected_->store(false);
            std::cout<<answer<<std::endl;
            std::cout<<"Disconnected due to Error try relogging"<<std::endl;
            std::cin.clear();
        }

    }
}

void SocketIO::MessageProcess(const std::string& message) {
    std::string sub="subscription:";
    std::string des="destination:";

    int pos = message.find(sub)+sub.length();
    std::string subscriptionId = message.substr(pos,message.find('\n',message.find(sub))-pos);

    pos = message.find(des)+des.length();

    std::string destination = message.substr(pos,message.find('\n',pos)-pos);
    std::string body = message.substr(message.rfind('\n',message.rfind('\n'))+1,message.rfind('\n'));
    std::string wishToBorrow = library.getWishToBorrow();


    //Wish to borrow Processessing
    if (body.find("wish to borrow")!=std::string::npos)
    {
        std::string userName = body.substr(0,body.find(' '));
        std::string bookToBorrow = body.substr(body.find("borrow")+7);

        if (library.findBook(destination,bookToBorrow)&&userName_!=userName){
            std::string frame="SEND\ndestination:"+destination+"\n\n"+userName_+" has "+bookToBorrow+"\n\0";
            connectionHandler.sendFrameAscii(frame,'\0');
        }
    }

    //Has the book Processessing
    else if(!wishToBorrow.empty()&&body.find("has "+wishToBorrow)!=std::string::npos)
    {
        std::string userName = body.substr(0,body.find(' '));
        if (userName!=userName_)
        {
            library.setWishToBorrow("");
            std::string frame =
                    "SEND\ndestination:" + destination + "\n\n" + "Taking " + wishToBorrow + " from " + userName +"\n\0";
            connectionHandler.sendFrameAscii(frame, '\0');
            library.addBook(destination, wishToBorrow, userName);
        }
    }

    //Taking Processessing
    else if(body.find("Taking")!=std::string::npos && body.rfind(userName_)!=std::string::npos)
    {
        std::string book=body.substr(7,body.find("from")-8);
        std::cout <<"removing: " << library.removeBook(destination,book)<< std::endl;
    }

    //Status Processessing
    else if (body.find("book status")!=std::string::npos)
    {
        std::string frame = "SEND\ndestination:" + destination + "\n\n" +userName_+':'+library.bookStatus(destination)+"\n\0";
        connectionHandler.sendFrameAscii(frame, '\0');

    }

    //Return Processessing
    else if (body.find("Returning")!=std::string::npos&&body.find(userName_)!=std::string::npos){
        std::string bookToAdd = body.substr(body.find(' ')+1,body.rfind("to")-body.find(' ')+1);
        library.addBook(destination,bookToAdd,library.findLender(bookToAdd));
    }



}

SocketIO::~SocketIO() {
    connected_=nullptr;
}
