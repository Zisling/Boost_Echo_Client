//
// Created by zisling on 04/01/2020.
//

#include "include/Books.h"
#include <utility>
#include <iostream>
#include <vector>
using namespace std;
Books::Books(mutex &mutex):map_(),_mutex(mutex) {
}

Books::~Books() {
    for (auto& a : map_) {
        delete a.second;
}
}


void Books::addBook(const std::string& genre, const std::string& book) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (map_[genre]== nullptr){
        map_[genre]= new vector<string>();;
    }
    map_[genre]->emplace_back(book);
}

bool Books::findBook(const std::string& genre,const std::string& book) {
    if (map_[genre]!= nullptr){
        for (const string& compare : *map_[genre]) {
            if (compare==book){
                return true;
            }
        }
    }
    return false;
}

string Books::removeBook(const std::string& genre,const std::string& book) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (map_[genre]!= nullptr){
        int posstion= 0;
        for (const string& compare : *map_[genre]) {
            if (compare==book){
                map_[genre]->erase(map_[genre]->begin()+posstion);
                return book;
            }
            posstion++;
        }
    }
    return "Book doesnt exist";
}
