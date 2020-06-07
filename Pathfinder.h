#pragma once
#include "Cell.h"
#include "Simple_window.h"
#include "Window.h"
#include "WaitWindow.h"
#include <cmath>
#include <stack>


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
    Vector<Cell*> searched;

    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();
    void click();
    void setStart(Loc l);
    void setEnd(Loc l);
    static void cb_start(Address, Address addr);
    void strt();
    static void cb_clear(Address, Address addr);
    void clear();
    Cell* openCell(Cell* cur,int xOffset,int yOffset);
    static void cb_maze(Address, Address addr);

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
    void mazeGen();

    Button dijkstraBt;
    Button clearBt;
    Button mazeBt;
};



