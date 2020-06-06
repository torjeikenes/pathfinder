#pragma once
#include "Cell.h"
#include "Simple_window.h"
#include "Window.h"
#include "WaitWindow.h"
#include <cmath>
#include <chrono>
#include <thread>


enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE};

class Pathfinder : public Graph_lib::Window{
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    //Wait_window win;
    //Graph_lib::Window win;
    Loc start;
    Loc end;
    Vector<Cell*> route;
    set<Cell*> q;
    Vector<Loc> blocked;
    bool running;
    bool clicked;
    void fillGrid();
    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();
    Loc pntToLoc(Point xy) {return Loc{xy.x/cellSize,xy.y/cellSize};}

    int Width() const { return xcell*cellSize;}
    int Height() const { return ycell*cellSize;}
    bool inRange(Point xy) const { return xy.x >= 0 && xy.x < Width() && xy.y >= 0 && xy.y < Height(); }

    void click();
    static void cb_start(Address, Address addr);
    void strt();
    //static void cb_click(Address, Address pw);
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});
    Cell* getCell(Loc l);
    //void wait_for_button() {win.wait_for_button();}
    void dijkstra();
    void drawPath(Cell* start,Cell* end);
    void drawBlocked();

    Button startbt;
};



