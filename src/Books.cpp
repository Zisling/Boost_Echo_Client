//
// Created by zisling on 04/01/2020.
//

#include "include/Books.h"
#include <utility>
#include <iostream>
#include <vector>
using namespace std;
Books::Books(std::mutex &mutex,std::mutex &mutex_Receipt,std::mutex &mutex_subId):
mapLibrary_(),mapReceipt(),subscriptionIDMap(), _mutex(mutex),_mutex_Receipt(mutex_Receipt),wishToBorrow_(),_mutex_subId(mutex_subId) {
}

Books::~Books() {
    for (auto& a : mapLibrary_) {
        delete a.second;
}
}


void Books::addBook(const std::string& genre, const std::string& book,const std::string& owner) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout <<"toadd:" << genre<< std::endl;

    if (mapLibrary_[genre] == nullptr){
        mapLibrary_[genre]= new vector<string>();;
    }
    if (mapBookLender_.find(book)!=mapBookLender_.end()){
    mapBookLender_[book]=owner;
    }else if (mapBookLender_[book]!=owner){
        mapBookLender_[book]=owner;
    }
    mapLibrary_[genre]->emplace_back(book);
}

bool Books::findBook(const std::string& genre,const std::string& book) {
    std::cout <<"tofind:"<<genre << std::endl;

    if (mapLibrary_[genre] != nullptr){
        for (const string& compare : *mapLibrary_[genre]) {
            if (compare==book){
                return true;
            }
        }
    }
    return false;
}

string Books::removeBook(const std::string& genre,const std::string& book) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (mapLibrary_[genre] != nullptr){
        int posstion= 0;
        for (const string& compare : *mapLibrary_[genre]) {
            if (compare==book){
                mapLibrary_[genre]->erase(mapLibrary_[genre]->begin() + posstion);
                return book;
            }
            posstion++;
        }
    }
    return "Book doesnt exist";
}

std::string Books::findLender(const std::string &book) {
    return mapBookLender_[book];
}
//todo see if there is a better way to make it more safe may !!!wchar!!!
const string &Books::getWishToBorrow() const {
    return wishToBorrow_;
}

void Books::setWishToBorrow(const string &wishToBorrow) {
    std::lock_guard<std::mutex> lock(_mutex);
    wishToBorrow_ = wishToBorrow;
}

std::string Books::bookStatus(const std::string& genre) {
    std::string out;
    for ( const string &book :*mapLibrary_[genre] ) {
        out+=book+',';
}
    out=out.substr(0,out.length()-1);
    return out;
}

void Books::addReceipt(int id, std::string action) {
    std::lock_guard<std::mutex> lock(_mutex_Receipt);
    std::string idStr = std::to_string(id);
    std::string numberOfZeros;
    for (int i = 0; i < 5-idStr.length(); ++i) {
        numberOfZeros+='0';
    }
    idStr=numberOfZeros+idStr;
    mapReceipt[idStr]=std::move(action);


}

std::string Books::getReceipt(const std::string& id) {
    std::lock_guard<std::mutex> lock(_mutex_Receipt);
    if (!mapReceipt[id].empty()){
        std::string out = mapReceipt[id];
        mapReceipt.erase(id);
        return out;
    }
    return "error receipt don't exist";
}

void Books::addId(const std::string& genre, int id) {
    std::lock_guard<std::mutex> lock(_mutex_subId);
    subscriptionIDMap[genre]=id;

}

void Books::addId(const std::string &genre, const std::string& id) {
    int idInt = std::stoi(id);
    addId(genre,idInt);
}

int Books::getId(const std::string& genre) {
    std::lock_guard<std::mutex> lock(_mutex_subId);
    if (subscriptionIDMap.find(genre)!=subscriptionIDMap.end()){
        return subscriptionIDMap[genre];
    } else{
        return -1;
    }
}

bool Books::removeId(const std::string& genre) {
    std::lock_guard<std::mutex> lock(_mutex_subId);
    if (subscriptionIDMap.find(genre)!=subscriptionIDMap.end()){
        subscriptionIDMap.erase(genre);
        return true;
    } else{
        return false;
    }
}



