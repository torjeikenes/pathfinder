#include "Cell.h"
#include "Pathfinder.h"

using namespace Graph_lib;
int main(){
    int xcell = 20;
    int ycell = 20;
    int cellSize = 20;
    Loc start{2,2};
    Loc end{8,7};
    Vector<Loc> blocked{Loc{5,0},Loc{5,1},Loc{5,2},Loc{4,3},Loc{4,4}};
    Pathfinder pf{xcell,ycell,cellSize,start,end,blocked};
    pf.drawBlocked();
    //pf.dijkstra();
    //pf.wait_for_button();
    //return gui_main();
}