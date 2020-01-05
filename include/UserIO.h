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

    void join(std::string genre);
    void exitgenre(std::string genre);
    void addbook(std::string genre,std::string bookname);
    void borrowbook(std::string genre,std::string bookname);
    std::string returnbook(std::string genre,std::string bookname);
    void status(std::string genre);
    void logout();

public:
    UserIO(const Books &library);

    void run(ConnectionHandler& connectionHandler);

    UserIO(Books& library);

    //login: in echoclient
    //join-genre
    //exit-genre
    //addbook-genre,bookname
    //borrowbook-genre,bookname
};


#endif //BOOST_ECHO_CLIENT_USERIO_H
