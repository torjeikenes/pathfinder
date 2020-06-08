#pragma once
#include "Graph.h"

using namespace Graph_lib;


enum class Stat{
    empty,
    visited,
    blocked
};

// struct for grid location
struct Loc{
    int x;
    int y;
    bool operator==(Loc& rhs){ return x==rhs.x && y == rhs.y;}
    Loc(int x, int y) : x{x}, y{y} {}
};


class Cell : public Graph_lib::Rectangle {
    Loc l;
    int distance;
    Cell* parent;
    Stat status;
public:
    Cell(Point xy, int ww, int hh,Loc l) 
        : Rectangle(xy,ww,hh),l{l},
        distance{std::numeric_limits<int>::max()},
        parent{nullptr},status{Stat::empty} { }
    void setBlocked();
    void setVisited();
    void setEmpty();
    void setDist(double d) {distance = d;}
    //void set_fill_color(Color c){pw->color(FL_RED);}
    double getDist() {return distance;}
    Stat getStatus() {return status;}
    Loc getLoc() {return l;}
    void SetParent(Cell* p) {parent = p;}
    Cell* getParent() {return parent;}
};
