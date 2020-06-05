#include "Cell.h"
#include "Pathfinder.h"

using namespace Graph_lib;
int main(){
    int xcell = 10;
    int ycell = 10;
    int cellSize = 20;
    Loc start{2,2};
    Loc end{8,7};
    Pathfinder pf{xcell,ycell,cellSize,start,end};
    pf.dijkstra();
    pf.wait_for_button();

}