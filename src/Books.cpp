//
// Created by zisling on 04/01/2020.
//

#include "include/Books.h"
#include <utility>
#include <iostream>
#include <vector>
using namespace std;
Books::Books(mutex &mutex): mapLibrary_(), _mutex(mutex),wishToBorrow_() {
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
    mapBookLender_[book]=owner;
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



