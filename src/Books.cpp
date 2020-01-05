//
// Created by zisling on 04/01/2020.
//

#include "include/Books.h"
#include <utility>
#include <iostream>
#include <vector>
using namespace std;
Books::Books(mutex &mutex): mapLibrary_(), _mutex(mutex) {
}

Books::~Books() {
    for (auto& a : mapLibrary_) {
        delete a.second;
}
}


void Books::addBook(const std::string& genre, const std::string& book) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (mapLibrary_[genre] == nullptr){
        mapLibrary_[genre]= new vector<string>();;
    }
    mapLibrary_[genre]->emplace_back(book);
}

bool Books::findBook(const std::string& genre,const std::string& book) {
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
