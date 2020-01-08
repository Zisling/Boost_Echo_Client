//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USERIO_H
#define BOOST_ECHO_CLIENT_USERIO_H


#include <mutex>
#include "Books.h"
#include "connectionHandler.h"

#include <boost/atomic.hpp>

class UserIO {
private:
    Books &library;

    std::map<std::string,int> subscriptionIDMap;
    std::string userName_;
    ConnectionHandler& connectionHandler;
    boost::atomic_bool *connected_;
public:
    void run();

    UserIO(Books &library, std::string userName, ConnectionHandler &connectionHandler,
           boost::atomic_bool *connected);





    //login: in echoclient
    //join-genre
    //exit-genre
    //addbook-genre,bookname
    //borrowbook-genre,bookname
};


#endif //BOOST_ECHO_CLIENT_USERIO_H
