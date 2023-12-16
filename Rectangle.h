#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using namespace std;

class Rectangle
{
private:
    int width;
    int height;
    char* color;

public:
    Rectangle(int width, int height, const char* color);
    Rectangle(const Rectangle& object);
    Rectangle();
    ~Rectangle();
    int PerCalc();
    void print();
    int get_size(); //get sizeof rectangle object
    Rectangle operator+(const Rectangle& object);
    Rectangle& operator=(const Rectangle& object);
    friend Rectangle operator-(const Rectangle& r1, const Rectangle& r2);
    Rectangle& operator++(); 
    Rectangle& operator++(int xd); 
    bool operator>=(Rectangle& object);
    friend ostream& operator<<(ostream& outf, const Rectangle& r);
    friend istream& operator>>(istream& inputf, Rectangle& r);


};
#endif