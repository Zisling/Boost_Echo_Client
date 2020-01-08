//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SOCKETIO_H
#define BOOST_ECHO_CLIENT_SOCKETIO_H


#include <string>
#include "connectionHandler.h"
#include <boost/atomic.hpp>


class SocketIO {
public:

    SocketIO(const std::string &userName, ConnectionHandler &connectionHandler, Books &library,
             boost::atomic_bool *connected);

    void run();

private:
    std::string userName_;
    ConnectionHandler& connectionHandler;
    Books &library;
    void MessageProcess(const std::string& message);
    boost::atomic_bool *connected_;



};


#endif //BOOST_ECHO_CLIENT_SOCKETIO_H
