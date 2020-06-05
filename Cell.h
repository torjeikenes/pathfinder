#pragma once
#include "Graph.h"

using namespace Graph_lib;

enum class Stat{
    empty,
    visited,
    blocked
};

struct Loc{
    int x;
    int y;
};

class Cell : public Rectangle {
    Loc l;
    double distance;
    Cell* parent;
    Stat status;
public:
    Cell(Point xy, int ww, int hh,Loc l) 
        : Rectangle(xy,ww,hh),l{l},
        distance{std::numeric_limits<double>::infinity()},
        parent{nullptr},status{Stat::empty} { }
    void setBlocked();
    void setVisited();
    void setEmpty();
    void setDist(double d) {distance = d;}
    double getDist() {return distance;}
    Stat getStatus() {return status;}
    Loc getLoc() {return l;}
    void SetParent(Cell* p) {parent = p;}
    Cell* getParent() {return parent;}
};


//struct cellComp {
//    bool operator() (Cell* lhs, Cell* rhs) const {
//        return lhs->getDist() < rhs->getDist();
//    }
//};