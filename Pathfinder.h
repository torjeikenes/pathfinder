#pragma once
#include <cmath>
#include <stack>
#include <queue>
#include "Window.h"
#include "GUI.h"
#include "Cell.h"


enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE};

// Cell comparator to sort the priority_queue
struct cellCompareGt{
    bool operator()(Cell* lhs, Cell* rhs);
};

using namespace Graph_lib;
class Pathfinder : public Graph_lib::Window{
public:
    Button dijkstraBt, AstBt, mazeBt, clearBt;
    Text moves;
    Rectangle mvBg;
    Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck=Vector<Loc>{0});

    void aStar(double waitTime);
    void dijkstra(double waitTime);
    void mazeGen(double waitTime);

    int manhattan(Cell* c);
    //clicks
    void handleClicks();

    //clear
    void clearBlk();
    void clear();
private:
    int xcell, ycell, cellSize, moveCtr;
    Vector<Loc> blocked;
    Vector_ref<Cell> vr;
    Vector<Cell*> route;
    Vector<Cell*> searched;
    priority_queue<Cell*,vector<Cell*>, cellCompareGt> q;
    Loc start;
    Loc end;
    bool running, startPress, endPress,dijkDone,astDone;
    Color searchC, pathC, startC, endC;

    // Dijkstra
    void strt();
    void compareCells(Cell* cur,int xOffset,int yOffset);
    Cell* getMinDist();

    // A*
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



