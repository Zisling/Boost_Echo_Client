//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SOCKETIO_H
#define BOOST_ECHO_CLIENT_SOCKETIO_H


#include <string>
#include "connectionHandler.h"

class SocketIO {
public:
    SocketIO(const std::string &userName, ConnectionHandler &connectionHandler, const Books &library);

    void run();

private:
    std::string userName_;
    ConnectionHandler& connectionHandler;
    Books library;
    void MessageProcess(const std::string& message);


};


#endif //BOOST_ECHO_CLIENT_SOCKETIO_H
