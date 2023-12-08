#ifndef BINARY_LIST_H
#define BINARY_LIST_H

#include <iostream>
#include <fstream>
using namespace std;
#pragma once

template <typename T>
class binary_list
{
public:
    binary_list();
    binary_list(const char filename[]);
    ~binary_list();
    void open_file(const char filename[]);
    void clear();
    void check_file();
    void push_front(const T value);
    void pop_front();
    void print();
    void insert_after(int index, const T value);
    void erase_after(int index);
    bool empty();
    T front();
    int length();
    void sort();

    class list_ex : public exception
    {
        const char* info;
    public:
        list_ex() noexcept { info = static_cast<const char*>("Exception had not thrown"); }
        list_ex(const char* info) noexcept { this->info = static_cast<const char*>(info); }
        const char* what() const noexcept override { return info; }
    };


private:
    void swap_neighbours_if_less(int prevpos);
    long int eof;
    int size;
    int head;
    fstream f;
};

#endif