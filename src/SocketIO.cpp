//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#include <include/Books.h>
#include <include/UserIO.h>
#include "SocketIO.h"

SocketIO::SocketIO(const std::string &userName, ConnectionHandler &connectionHandler, const Books &library,
                   boost::atomic_bool *connected) : userName_(userName), connectionHandler(connectionHandler),
                                                    library(library), connected_(connected) {}


void SocketIO::run() {
    std::string answer;
    while (*connected_){
        answer.clear();
        if (!connectionHandler.getFrameAscii(answer,'\0')) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        std::cout <<answer << std::endl;

        answer.resize(answer.length()-1);
        if (answer.find("MESSAGE")!=std::string::npos){
            MessageProcess(answer);
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
    std::cout << " look at me "<<destination << std::endl;
    std::cout << " end of look at me" << std::endl;

    std::string body = message.substr(message.rfind('\n',message.rfind('\n'))+1,message.rfind('\n'));
    std::string wishToBorrow = library.getWishToBorrow();
    if (body.find("wish to borrow")!=std::string::npos){
        std::string userName = body.substr(0,body.find(' '));
        std::string bookToBorrow = body.substr(body.find("borrow")+7);
        std::cout <<library.findBook(destination,bookToBorrow) << std::endl;

        if (library.findBook(destination,bookToBorrow)&&userName_!=userName){
            std::string frame="SEND\ndestination:"+destination+"\n\n"+userName_+" has "+bookToBorrow+"\n\0";
            connectionHandler.sendFrameAscii(frame,'\0');
        }
    }

    else if(!wishToBorrow.empty()&&body.find("has "+wishToBorrow)!=std::string::npos){
        std::string userName = body.substr(0,body.find(' '));
        if (userName!=userName_) {
            library.setWishToBorrow("");
            std::string frame =
                    "SEND\ndestination:" + destination + "\n\n" + "Taking " + wishToBorrow + " from " + userName +
                    "\n\0";
            connectionHandler.sendFrameAscii(frame, '\0');
            library.addBook(destination, wishToBorrow, userName);
        }
    } else if(body.find("Taking")!=std::string::npos && body.rfind(userName_)!=std::string::npos){
        std::cout <<"removeing: " << library.removeBook(destination,body.substr(7,body.find("from")))<< std::endl;
    }


}
