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
    int moveCtr;
    Color searchC;
    Color pathC; 
    Color startC;
    Color endC;

    void strt();
    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();

    int manhattan(Cell* c);
    void compareCellsAst(Cell* cur,int xOffset,int yOffset);
    Cell* getMinCost();

    Cell* openCell(Cell* cur,int xOffset,int yOffset);

    void clear();
    void click();
    static void cb_start(Address, Address addr);
    static void cb_clear(Address, Address addr);
    static void cb_maze(Address, Address addr);
    static void cb_ast(Address, Address addr);

    void addMove();
    void setStart(Loc l);
    void setEnd(Loc l);
    int Width() const { return xcell*cellSize;}
    int Height() const { return ycell*cellSize;}
    bool inRange(Point xy) const { return xy.x >= 0 && xy.x < Width() && xy.y >= 0 && xy.y < Height(); }
    Loc pntToLoc(Point xy) {return Loc{xy.x/cellSize,xy.y/cellSize};}
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});
    Cell* getCell(Loc l);

    void dijkstra(double waitTime);
    void aStar(double waitTime);
    void mazeGen(double waitTime);

    void drawPath(Cell* start,Cell* end);
    void handleClicks();
    void clearBlk();

    Button dijkstraBt;
    Button AstBt;
    Button mazeBt;
    Button clearBt;
    Text moves;
    Rectangle mvBg;
};



