//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USERIO_H
#define BOOST_ECHO_CLIENT_USERIO_H


#include <mutex>
#include "Books.h"
#include "connectionHandler.h"

class UserIO {
private:
    Books library;
    std::map<std::string,int> subscriptionIDMap;
    std::string userName_;
    ConnectionHandler& connectionHandler;
    void join(std::string genre);
    void exitgenre(std::string genre);
    void addbook(std::string genre,std::string bookname);
    void borrowbook(std::string genre,std::string bookname);
    std::string returnbook(std::string genre,std::string bookname);
    void status(std::string genre);
    void logout();

public:

    void run();

    UserIO(const Books& library,std::string& userName,ConnectionHandler& connectionHandler);

    //login: in echoclient
    //join-genre
    //exit-genre
    //addbook-genre,bookname
    //borrowbook-genre,bookname
};


#endif //BOOST_ECHO_CLIENT_USERIO_H
