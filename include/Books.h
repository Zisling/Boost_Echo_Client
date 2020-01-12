
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
    std::map<std::string,std::string> mapReceipt;
    std::map<std::string,int> subscriptionIDMap;
    std::mutex & _mutex;
    std::mutex & _mutex_Receipt;
    std::mutex & _mutex_subId;

public:
    //Constructor
    Books(std::mutex &mutex,std::mutex &mutex_Receipt,std::mutex &mutex_subId);
    //Copy Constructor
    Books(const Books&);

    void addReceipt(int id,std::string action);
    std::string getReceipt(const std::string& id);





    std::string findLender(const std::string& book);
    void addBook(const std::string& genre,const std::string& book,const std::string& owner);
    bool findBook(const std::string& genre,const std::string& book);
    std::string removeBook(const std::string& genre,const std::string& book);

    void addId(const std::string& genre,int id);
    void addId(const std::string& genre,const std::string& id);
    int getId(const std::string& genre);
    bool removeId(const std::string& genre);



    const std::string &getWishToBorrow() const;

    std::string bookStatus(const std::string& genre);

    void setWishToBorrow(const std::string &wishToBorrow);

    virtual ~Books();
};


#endif //BOOST_ECHO_CLIENT_BOOKS_H
