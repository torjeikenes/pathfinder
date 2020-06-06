#pragma once
#include "Cell.h"
#include "Simple_window.h"
#include "Window.h"
#include "WaitWindow.h"
#include <cmath>


class Pathfinder {
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    Wait_window win;
    //Graph_lib::Window win;
    Loc start;
    Loc end;
    Vector<Cell*> route;
    Vector<Loc> blocked;
    void fillGrid();
    void compareCells(Cell* cur,int xOffset,int yOffset);
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});
    Cell* getCell(Loc l);
    void wait_for_button() {win.wait_for_button();}
    void dijkstra();
    void drawPath(Cell* start,Cell* end);
};

Cell* getMinDist(set<Cell*>& q);

