//

#include "include/UserIO.h"
#include <utility>


UserIO::UserIO(Books &library, std::string userName,
               ConnectionHandler &connectionHandler, boost::atomic_bool *connected) : library(library),
                                                                                      subscriptionIDMap(
                                                                                              ),
                                                                                      userName_(std::move(userName)),
                                                                                      connectionHandler(
                                                                                              connectionHandler),
                                                                                      connected_(connected) {}


//
void UserIO::run() {

    int counterIDsub=0;




    while (*connected_) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);

        //Join frame
        if (line.find("join", 0)!=std::string::npos)
        {
            //Extracting genre from the given string.
            std::string genre=line.substr(5);
            //Framing subscribe frame
            std::string frame="SUBSCRIBE\ndestination:"+genre+"\nid:"+std::to_string(counterIDsub)+"\n\n\0";

            subscriptionIDMap[genre]=counterIDsub;
            counterIDsub++;


            //Sending frame
            connectionHandler.sendFrameAscii(frame,'\0');

        }
        //Exit frame
        else if(line.find("exit",0)!=std::string::npos)
        {
            //Extracting genre from the given string.
            std::string genre=line.substr(4);


            //Framing unsubscribe frame
            std::string frame="UNSUBSCRIBE\nid:"+std::to_string(subscriptionIDMap[genre])+"\n\n\0";
            std::cout<<frame<<std::endl;

            //Sending frame
            connectionHandler.sendFrameAscii(frame,'\0');

        }

        //Add book frame
        else if(line.find("add",0)!=std::string::npos)
        {
            //Extracting genre from the given string.
            std::string withoutAdd=line.substr(4);
            if(withoutAdd.find(' ',0)!=std::string::npos)
            {

                std::string genre=withoutAdd.substr(0,withoutAdd.find(' ',0));
                std::string bookname=withoutAdd.substr(withoutAdd.find(' ')+1);

                library.addBook(genre,bookname,userName_);


                //Framing Send Frame
                std::string frame="SEND\ndestination:"+genre+"\n\n"+userName_+" has added the book "+bookname+"\n\0";

                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

            } else
            {
                std::cout<<"Add didn't succeed,Command is corrupted."<<std::endl;
            }
        }
        else if(line.find("borrow",0)!=std::string::npos){

            std::string withoutBorrow=line.substr(7);
            if(withoutBorrow.find(' ',0)!=std::string::npos) {

                std::string genre = withoutBorrow.substr(0, withoutBorrow.find(' ', 0));
                std::string bookname = withoutBorrow.substr(withoutBorrow.find(' ', 0) + 1);
                //Framing Send Frame
                library.setWishToBorrow(bookname);/// user set the book he wish to get
                std::string frame="SEND\ndestination:"+genre+"\n\n"+userName_+" wish to borrow "+bookname+"\n\0";
                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

            } else
            {
                std::cout<<"Borrow didn't succeed,Command is corrupted."<<std::endl;
            }

        }
        else if(line.find("return")!=std::string::npos) {
            std::string withoutReturn = line.substr(7);
            if (withoutReturn.find(' ', 0) != std::string::npos) {

                std::string genre = withoutReturn.substr(0, withoutReturn.find(' ', 0));
                std::string bookname = withoutReturn.substr(withoutReturn.find(' ', 0) + 1);

                std::string lender=library.findLender(bookname);

                //Framing Send Frame
                std::string frame="SEND\ndestination:"+genre+"\nReturning "+bookname+" to"+lender+"\n\0";
                library.removeBook(genre,bookname);
                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

            }
            else {
                std::cout << "Return didn't succeed,Command is corrupted." << std::endl;

            }
        }
        else if(line.find("status")!=std::string::npos)
        {
            std::string withoutStatus=line.substr(7);

                std::string genre = withoutStatus;

                //Framing Send Frame
                std::string frame="SEND\ndestination:"+genre+"\n\nbook status"+"\n\0";

                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

        }

        else if(line.find("logout")!=std::string::npos)
        {
                //Framing Disconnect frame
                std::string frame="DISCONNECT\nreceipt:42\n\n\0";
                //Sending frame
            connectionHandler.sendFrameAscii(frame,'\0');
//            disconnected_=true;
        } else if(!line.empty())
        {
            std::cout<<"Command is Corrupted or not supported."<<std::endl;
            std::cout<<"Re-enter your command."<<std::endl;

        }


    }
}




