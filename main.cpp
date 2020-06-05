#include "Cell.h"
#include "Pathfinder.h"

using namespace Graph_lib;
int main(){
    int xcell = 10;
    int ycell = 10;
    int cellSize = 20;
    //Simple_window win(Point{100,100},xcell*cellSize,ycell*cellSize,"Pathfinder");
    //Vector_ref<Cell> vr;
    Pathfinder pf{xcell,ycell,cellSize};
    auto c = pf.getCell(Loc{2,2});
    c->set_fill_color(Color::blue);
    pf.wait_for_button();

}