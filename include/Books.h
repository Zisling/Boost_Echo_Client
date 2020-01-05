//
// Created by zisling on 04/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOKS_H
#define BOOST_ECHO_CLIENT_BOOKS_H
#include <iostream>
#include <vector>
#include <mutex>
#include <boost/asio.hpp>


class Books {
private:

    std::map<std::string,std::vector<std::string>*> mapLibrary_;
    std::map<std::string ,std::string> mapBookLender_;
    std::mutex & _mutex;
public:
    Books(std::mutex &mutex);

    std::string findLender(const std::string& book);
    void addBook(const std::string& genre,const std::string& book);
    bool findBook(const std::string& genre,const std::string& book);
    std::string removeBook(const std::string& genre,const std::string& book);

    virtual ~Books();
};


#endif //BOOST_ECHO_CLIENT_BOOKS_H
