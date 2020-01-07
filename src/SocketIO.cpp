//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#include <include/Books.h>
#include <include/UserIO.h>
#include "SocketIO.h"

SocketIO::SocketIO(const std::string &userName, ConnectionHandler &connectionHandler, const Books &library,
                   boost::atomic_bool *connected) : userName_(userName), connectionHandler(connectionHandler),
                                                    library(library), connected_(connected) {}


void SocketIO::run() {
    std::string answer;
    while (*connected_){
        if (!connectionHandler.getFrameAscii(answer,'\0')) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        answer.resize(answer.length()-1);
        if (answer.find("MESSAGE")!=std::string::npos){
            MessageProcess(answer);
        }

    }
}

void SocketIO::MessageProcess(const std::string& message) {
    std::string sub="subscription:";
    std::string des="destination:";
    std::string subscriptionId = message.substr(message.find(sub)+sub.length(),message.find('\n',message.find(sub)));
    std::string destination = message.substr(message.find(des)+sub.length(),message.find('\n',message.find(des)));
//    std::string body = message.substr(message.rfind('\n',message.rfind('\n'))+1,)


}
