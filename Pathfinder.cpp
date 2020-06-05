#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s,Loc start, Loc end)
    : xcell{x},ycell{y},cellSize{s},
    win{Point{100,100},xcell*cellSize,ycell*cellSize,"Pathfinder"}, 
    start{start},end{end}
    {
    fillGrid();
    getCell(start)->set_fill_color(Color::magenta);
    getCell(end)->set_fill_color(Color::green);

}

//Fills grid with cells and attaches to vr
void Pathfinder::fillGrid(){
    for (int j = 0;j<xcell;j++){
        for (int i = 0;i<ycell;i++){
            vr.push_back(new Cell{Point{i*cellSize,j*cellSize},cellSize,cellSize,Loc{i,j}});
            win.attach(vr[vr.size()-1]);
        }
    }
}

// Returns pointer to Cell with given loc
Cell* Pathfinder::getCell(Loc l){
    //Returns nullptr if outside of grid
    try{
        int i=l.y*xcell+l.x;
        if(l.x>=xcell || l.y >=ycell){
            throw range_error("Out of bounds");
        }
        return &vr[i];
    }
    catch (exception& e){
        std::cerr<<e.what();
        return nullptr;
    }
}

void Pathfinder::dijkstra(){ 
    auto cur = getCell(start);
    cur->setDist(0);

    set<Cell*> q;
    for(auto e:vr){
        q.insert(e);
    };


    while(getCell(end)->getStatus()!=Stat::visited){

        cur = getMinDist(q);

        compareCells(cur,1,0); // right
        compareCells(cur,0,1); // down
        compareCells(cur,-1,0);// left
        compareCells(cur,0,-1);// up

        cur->setVisited();
        q.erase(cur);
        getCell(start)->set_fill_color(Color::magenta);
    }
}

// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCells(Cell* cur,int xOffset,int yOffset){
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        // assignes current distance + 1 if it is lower
        if(cur->getDist()+1<next->getDist()){
            next->setDist(cur->getDist()+1);
        }
    }
}


Cell* getMinDist(set<Cell*>& q){
    auto e = *(q.begin());
    for(auto c:q){
        if(c->getDist()<e->getDist()){
            e = c;
        }
    }
    return e;
}

