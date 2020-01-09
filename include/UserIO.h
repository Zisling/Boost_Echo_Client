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

    std::string userName_;
    ConnectionHandler& connectionHandler;
    boost::atomic_bool *connected_;


    int exit(const std::string &line, int counterIDReceipt);
    void join(const std::string &line, int counterIDReceipt, int counterIDsub);
    void addbook(const std::string& line);
    void borrowbook(std::string line);
    void status(const std::string& line);
    void returnbook(const std::string& line);
    void logout(const std::string& line,int);

public:
    void run();

    UserIO(Books &library, std::string userName, ConnectionHandler &connectionHandler,boost::atomic_bool *connected);






    //login: in echoclient
    //join-genre
    //exit-genre
    //addbook-genre,bookname
    //borrowbook-genre,bookname
};


#endif //BOOST_ECHO_CLIENT_USERIO_H
