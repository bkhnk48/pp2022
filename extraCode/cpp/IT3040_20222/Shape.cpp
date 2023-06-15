#include <iostream>
#include <string>
#include "Point.cpp"
#pragma once
using namespace std;

class Shape
{
public:
    Point *start;
    Point *end;
    double d;
    string name;

    void setName(string name)
    {
        this->name = name;
        if(name.find("E") == std::string::npos
             && name.find("J") == std::string::npos){
            cout<<"DEBUG: "<<__FILE__<<":"<<__LINE__<<":"<<endl;
        }
    }

    void setPoint(Point *start, Point *end)
    {
        this->start = start;
        this->end = end;
    }

    bool equals(Shape *s)
    {
        return ((this->start)->equals(s->start) && (this->end)->equals(s->end));
    }
};
