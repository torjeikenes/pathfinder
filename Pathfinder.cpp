#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s) 
    : xcell{x},ycell{y},cellSize{s},
    win{Point{100,100},xcell*cellSize,ycell*cellSize,"Pathfinder"}
    {
    fillGrid();
    vr[15].set_fill_color(Color::green);
    vr[44].set_fill_color(Color::red);
}


void Pathfinder::fillGrid(){
    //for (int i = 0;i<w;i++){
    //    Vector_ref<Cell> vx;
    //    for (int j = 0;j<h;j++){
    //        vx.push_back(new Cell{Point{i*w,j*h},cellSize,cellSize,Loc{i,j}});
    //    }
    //    vr.push_back(vx);
    //    
    //}
    for (int j = 0;j<xcell;j++){
        for (int i = 0;i<ycell;i++){
            vr.push_back(new Cell{Point{i*cellSize,j*cellSize},cellSize,cellSize,Loc{i,j}});
            win.attach(vr[vr.size()-1]);
        }
    }

    //for (int i = 0;i<w;i++){
    //    for (int j = 0;j<h;j++){
    //        win.attach(vr[i][j]);
    //    }
    //}
}


Cell* Pathfinder::getCell(Loc l){
    int i=l.y*xcell+l.x;
    return &vr[i];
}