#pragma once
#include "Cell.h"
#include "Simple_window.h"
#include "Window.h"
#include "WaitWindow.h"
#include <cmath>


enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE};

class Pathfinder : public Graph_lib::Window{
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    Loc start;
    Loc end;
    Vector<Loc> blocked;
    bool running;
    bool startPress;
    bool endPress;
    Vector<Cell*> route;
    set<Cell*> q;
    Vector<Cell*> visited;

    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();
    void click();
    void setStart(Loc l);
    void setEnd(Loc l);
    static void cb_start(Address, Address addr);
    void strt();
    static void cb_clear(Address, Address addr);
    void clear();

    int Width() const { return xcell*cellSize;}
    int Height() const { return ycell*cellSize;}
    bool inRange(Point xy) const { return xy.x >= 0 && xy.x < Width() && xy.y >= 0 && xy.y < Height(); }
    Loc pntToLoc(Point xy) {return Loc{xy.x/cellSize,xy.y/cellSize};}
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});
    Cell* getCell(Loc l);
    void dijkstra();
    void drawPath(Cell* start,Cell* end);
    void handleClicks();

    Button dijkstraBt;
    Button clearBt;
};



