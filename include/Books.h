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
    std::string wishToBorrow_;
    std::map<std::string,std::vector<std::string>*> mapLibrary_;
    std::map<std::string ,std::string> mapBookLender_;
    std::mutex & _mutex;
public:
    Books(std::mutex &mutex);

    std::string findLender(const std::string& book);
    void addBook(const std::string& genre,const std::string& book,const std::string& owner);
    bool findBook(const std::string& genre,const std::string& book);
    std::string removeBook(const std::string& genre,const std::string& book);

    const std::string &getWishToBorrow() const;

    std::string bookStatus(const std::string& genre);

    void setWishToBorrow(const std::string &wishToBorrow);

    virtual ~Books();
};


#endif //BOOST_ECHO_CLIENT_BOOKS_H
