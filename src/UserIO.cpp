//

#include "UserIO.h"

#include <utility>


UserIO::UserIO(const Books &library, std::string  &userName) : library(library),subscriptionIDMap(),userName_(std::move(userName)) {}

//
void UserIO::run(ConnectionHandler& connectionHandler) {

    bool disconnected = false;
    int counterIDsub=0;


    while (!disconnected) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);

        //Join frame
        if (line.find("join", 0)!=std::string::npos)
        {
            //Extracting genre from the given string.
            std::string genre=line.substr(4);
            //Framing subscribe frame
            std::string frame="SUBSCRIBE\ndestination:/"+genre+"\nid:"+std::to_string(counterIDsub)+"\n\n\0";

            subscriptionIDMap[genre]=counterIDsub;
            counterIDsub++;

            std::cout<<frame<<std::endl;

            //Sending frame
            connectionHandler.sendFrameAscii(frame,'\0');

        }
        //Exit frame
        else if(line.find("exit",0)!=std::string::npos)
        {
            //Extracting genre from the given string.
            std::string genre=line.substr(4);
            std::cout<<genre<<std::endl;


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
            std::string withoutAdd=line.substr(3);
            if(withoutAdd.find(' ',0)!=std::string::npos)
            {

                std::string genre=withoutAdd.substr(0,withoutAdd.find(' ',0));
                std::string bookname=withoutAdd.substr(withoutAdd.find(' ',0)+1);

                library.addBook(genre,bookname,userName_);

                std::cout<<genre<<std::endl;
                std::cout<<bookname<<std::endl;

                //Framing Send Frame
                std::string frame="SEND\ndestination:"+genre+"\n\n"+userName_+" has added the book"+bookname+"\n\0";
                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

            } else
            {
                std::cout<<"Add didn't succeed,Frame is corrupted."<<std::endl;
            }
        }
        else if(line.find("borrow",0)!=std::string::npos){

            std::string withoutBorrow=line.substr(6);
            if(withoutBorrow.find(' ',0)!=std::string::npos) {

                std::string genre = withoutBorrow.substr(0, withoutBorrow.find(' ', 0));
                std::string bookname = withoutBorrow.substr(withoutBorrow.find(' ', 0) + 1);
                //Framing Send Frame
                std::string frame="SEND\ndestination:"+genre+"\n\n"+userName_+" wish to borrow"+bookname+"\n\0";
                //Sending frame
                connectionHandler.sendFrameAscii(frame,'\0');

            } else
            {
                std::cout<<"Borrow didn't succeed,Frame is corrupted."<<std::endl;

            }
        }
        else if(line.find(""))



                int len = line.length();
        if (!connectionHandler.sendLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            disconnected = true;
        }
    }
}

void UserIO::join(std::string genre) {


}

void UserIO::exitgenre(std::string genre) {

}

void UserIO::addbook(std::string genre, std::string bookname) {

}

void UserIO::borrowbook(std::string genre, std::string bookname) {

}

std::string UserIO::returnbook(std::string genre, std::string bookname) {
    return std::__cxx11::string();
}

void UserIO::status(std::string genre) {

}

void UserIO::logout() {

}


