#pragma once
#include "Graph.h"
#include "Simple_window.h"

using namespace Graph_lib;

enum class Stat{
    closed=0,
    open
};

struct Loc{
    int x;
    int y;
};

class Cell : public Rectangle {
    Loc l;
public:
    Cell(Point xy, int ww, int hh,Loc l) : Rectangle(xy,ww,hh),l{l} { }
    
};