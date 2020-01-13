
#ifndef BOOST_ECHO_CLIENT_SOCKETIO_H
#define BOOST_ECHO_CLIENT_SOCKETIO_H


#include <string>
#include "../include/connectionHandler.h"
#include <boost/atomic.hpp>
#include <thread>


class SocketIO {
public:

    SocketIO(std::string userName, ConnectionHandler &connectionHandler, Books &library,
             boost::atomic_bool *connected);

    SocketIO(const SocketIO&);

    SocketIO&operator=(const SocketIO&);


    virtual ~SocketIO();

    void run();

private:
    std::string userName_;
    ConnectionHandler& connectionHandler;
    Books &library;
    void MessageProcess(const std::string& message);
    boost::atomic_bool *connected_;



};


#endif //BOOST_ECHO_CLIENT_SOCKETIO_H
