//
// Created by nirohan@wincs.cs.bgu.ac.il on 05/01/2020.
//

#include "UserIO.h"

UserIO::UserIO(const Books &library) : library(library) {}

void UserIO::run(ConnectionHandler& connectionHandler) {

    bool disconnected = false;

    while (!disconnected) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        if (line.find("join", 0))
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


