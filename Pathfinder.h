#pragma once
#include "Cell.h"
#include "Simple_window.h"
#include "Window.h"
#include "WaitWindow.h"
#include <cmath>
#include <stack>


enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE};

class Pathfinder : public Graph_lib::Window{
public:
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});

    void dijkstra(double waitTime);
    void aStar(double waitTime);
    void mazeGen(double waitTime);

    //clicks
    void handleClicks();

    //clear
    void clearBlk();
    void clear();

    Button dijkstraBt, AstBt, mazeBt, clearBt;
    Text moves;
    Rectangle mvBg;
private:
    int xcell, ycell, cellSize;
    Vector_ref<Cell> vr;
    Loc start;
    Loc end;
    Vector<Loc> blocked;
    bool running, startPress, endPress;
    Vector<Cell*> route;
    set<Cell*> q;
    Vector<Cell*> searched;
    int moveCtr;
    Color searchC, pathC, startC, endC;

    // Dijkstra
    void strt();
    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();

    // A*
    int manhattan(Cell* c);
    void compareCellsAst(Cell* cur,int xOffset,int yOffset);
    Cell* getMinCost();

    void drawPath(Cell* start,Cell* end);

    // Maze
    Cell* openCell(Cell* cur,int xOffset,int yOffset);


    //clicks
    void click();

    //set get
    void setStart(Loc l);
    void setEnd(Loc l);
    Cell* getCell(Loc l);

    // Callback
    static void cb_start(Address, Address addr);
    static void cb_clear(Address, Address addr);
    static void cb_maze(Address, Address addr);
    static void cb_ast(Address, Address addr);


    void addMove();

    
    int Width() const { return xcell*cellSize;}
    int Height() const { return ycell*cellSize;}
    bool inRange(Point xy) const { return xy.x >= 0 && xy.x < Width() && xy.y >= 0 && xy.y < Height(); }
    Loc pntToLoc(Point xy) {return Loc{xy.x/cellSize,xy.y/cellSize};}
};



