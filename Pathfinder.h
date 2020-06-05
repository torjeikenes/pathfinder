#pragma once
#include "Cell.h"

class Pathfinder {
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    Simple_window win;
    //Simple_window win(Point{100,100},xcell*cellSize,ycell*cellSize,"Pathfinder");
public:
    Pathfinder(int x, int y, int s);
    void fillGrid();
    Cell* getCell(Loc l);
    void wait_for_button() {win.wait_for_button();}
};