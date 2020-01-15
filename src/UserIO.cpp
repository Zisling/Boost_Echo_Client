//

#include "../include/UserIO.h"

#include <utility>

#include <boost/thread/thread.hpp>
UserIO::UserIO(Books &library, std::string userName,
               ConnectionHandler &connectionHandler, boost::atomic_bool *connected) :
               library(library),userName_(std::move(userName)),connectionHandler(connectionHandler),connected_(connected) {}


UserIO::UserIO(const UserIO &otherUserIO):
library(otherUserIO.library),userName_(otherUserIO.userName_),connectionHandler(otherUserIO.connectionHandler),connected_(otherUserIO.connected_){}


UserIO &UserIO::operator=(const UserIO & other) {
    library=other.library;
    userName_=other.userName_;
    connected_=other.connected_;

    return *this;
}


/**
 * Run method of userIO
 * @param line-shared object between both threads.
 */

void UserIO::run(std::string &line) {

    int counterIDsub=0;
    int counterIDReceipt=1;

    while (connected_->load())
    {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line2(buf);
        line=std::move(line2);

        if (!connected_->load()){
            break;
        }
        //Join frame
        if (line.length()>4&&line.substr(0,4)=="join")
        {
            UserIO::join(line,counterIDReceipt,counterIDsub);
            counterIDsub++;
            counterIDReceipt++;
        }
        //Exit frame
        else if(line.length()>4&&line.substr(0,4)=="exit")
        {
            counterIDReceipt=UserIO::exit(line,counterIDReceipt);
            counterIDReceipt++;
        }

        //Add book frame
        else if(line.length()>3&&line.substr(0,3)=="add")
        {
            UserIO::addbook(line);
        }

        //Borrow frame
        else if(line.length()>6&&line.substr(0,6)=="borrow")
        {
            UserIO::borrowbook(line);
        }
        //Return book frame
        else if(line.length()>6&&line.substr(0,6)=="return") {
            UserIO::returnbook(line);
        }
        //Status frame
        else if(line.length()>6&&line.substr(0,6)=="status")
        {
            UserIO::status(line);
        }
        //Logout frame (Disconnect)
        else if(line=="logout")
        {
            UserIO::logout(line,counterIDReceipt);
            line.clear();
            break;
        }
        //Empty line reply
        else if(!line.empty())
        {
            std::cout<<"Command is Corrupted or not supported."<<std::endl;
            std::cout<<"Re-enter your command."<<std::endl;

        }

        std::cin.clear();
        fflush(stdin);
    }
}

void UserIO::join(const std::string& line,int counterIDReceipt,int counterIDsub) {

    //Extracting genre from the given string.
    std::string genre=line.substr(5);
    //Framing subscribe frame
    library.addReceipt(counterIDReceipt, "Joined club "+genre+"\nIDsub:"+std::to_string(counterIDsub) );
    std::string frame="SUBSCRIBE\ndestination:"+genre+"\nid:"+std::to_string(counterIDsub)+"\nreceipt:"+std::to_string(counterIDReceipt)+"\n\n\0";

    //Sending frame
    connectionHandler.sendFrameAscii(frame,'\0');

}

int UserIO::exit(const std::string& line, int counterIDReceipt) {

    //Extracting genre from the given string.
    std::string genre=line.substr(5);

    //Framing unsubscribe frame
    library.addReceipt(counterIDReceipt,"Exited club "+genre);
    int id =library.getId(genre);
    //if indeed subscribed to topic id is not equal to -1
    if (id!=-1){
        std::string frame="UNSUBSCRIBE\nid:"+std::to_string(id)+"\nreceipt:"+std::to_string(counterIDReceipt)+"\n\n\0";

        //Sending frame
        connectionHandler.sendFrameAscii(frame,'\0');
    } else{
        std::cout<<"Error:Not Subscribed to this genre."<<std::endl;
    }
    return counterIDReceipt;
}

void UserIO::addbook(const std::string& line) {

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

void UserIO::borrowbook(std::string line) {

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

void UserIO::status(const std::string& line) {

    std::string withoutStatus=line.substr(7);

    const std::string& genre = withoutStatus;

    //Framing Send Frame
    std::string frame="SEND\ndestination:"+genre+"\n\nbook status"+"\n\0";

    //Sending frame
    connectionHandler.sendFrameAscii(frame,'\0');

}

void UserIO::returnbook(const std::string &line) {
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

void UserIO::logout(const std::string& line, int counterIDReceipt) {
    library.addReceipt(counterIDReceipt,"DISCONNECT");
    //Framing Disconnect frame
    std::string frame="DISCONNECT\nreceipt:"+std::to_string(counterIDReceipt)+"\n\n\0";
    //Sending frame
    connectionHandler.sendFrameAscii(frame,'\0');

}



