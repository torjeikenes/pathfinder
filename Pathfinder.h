#pragma once
#include "Cell.h"

class Pathfinder {
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    Simple_window win;
    Loc start;
    Loc end;
    Vector<Cell*> route;
    void fillGrid();
    void compareCells(Cell* cur,int xOffset,int yOffset);
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end);
    Cell* getCell(Loc l);
    void wait_for_button() {win.wait_for_button();}
    void dijkstra();
};


