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
    int cost;
    Cell* parent;
    Stat status;
    Color blockC;
    Color emptyC;
    Color visitedC;
public:
    Cell(Point xy, int ww, int hh,Loc l) 
        : Rectangle(xy,ww,hh),l{l},
        distance{std::numeric_limits<int>::max()},
        cost{std::numeric_limits<int>::max()},
        parent{nullptr},status{Stat::empty},
        blockC{Color::black},emptyC{Color::gray},visitedC{Color::cyan} {}
    void setBlocked();
    void setVisited();
    void setEmpty();
    void setDist(int d) {distance = d;}
    int getDist() {return distance;}
    void setCost(int c) {cost = c;}
    int getCost() {return cost;}
    Stat getStatus() {return status;}
    Loc getLoc() {return l;}
    void SetParent(Cell* p) {parent = p;}
    Cell* getParent() {return parent;}
    bool operator<(Cell* rhs);
};
