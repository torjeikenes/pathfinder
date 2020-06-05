#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck)
    : xcell{x},ycell{y},cellSize{s},
    win{Point{100,100},xcell*cellSize,ycell*cellSize,"Pathfinder"}, 
    start{start},end{end},blocked{blck}
    {
    //Fills up the grid
    fillGrid();
    //Color start and end
    getCell(start)->set_fill_color(Color::magenta);
    getCell(end)->set_fill_color(Color::green);
    //Blocks all cells from the blck vector
    for(auto b:blocked){
        getCell(b)->setBlocked();
    }

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
        if(l.x>=xcell || l.x<0 || l.y >=ycell || l.y < 0){
            throw range_error("Out of bounds");
        }
        return &vr[i];
    }
    catch (exception& e){
        //std::cerr<<e.what();
        return nullptr;
    }
}

void Pathfinder::dijkstra(){ 
    // the current cell is set to the start location
    auto cur = getCell(start);
    //curs distance is set to 0. The rest are infinite from the constructor
    cur->setDist(0);

    // Makes set with all Cells
    set<Cell*> q;
    for(auto e:vr){
        q.insert(e);
    };

    //start window and wait 1 sec
    win.show();
    win.wait(1);


    //Runs until the end cell has been visited
    while(getCell(end)->getStatus()!=Stat::visited){
        //The current cell is set to the cell with the lowest distance in q
        cur = getMinDist(q);

        //checks all directions
        compareCells(cur,-1,-1);// up left
        compareCells(cur,0,-1);// up
        compareCells(cur,1,-1);// up right
        compareCells(cur,1,0); // right
        compareCells(cur,1,1); // down right
        compareCells(cur,0,1); // down
        compareCells(cur,-1,1); // down left
        compareCells(cur,-1,0);// left

        // Done with current so its set to visited and removed from q
        cur->setVisited();
        q.erase(cur);
        getCell(start)->set_fill_color(Color::magenta);
        //redraws window
        win.redraw();
        //waits for 0.25 seconds
        win.wait(0.25); 
    }
    //draws path from end to start
    drawPath(cur,getCell(start));
    getCell(end)->set_fill_color(Color::green);
    win.redraw();
    
    win.wait_for_button();
}

// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCells(Cell* cur,int xOffset,int yOffset){
    double dist =  sqrt(abs(xOffset)+abs(yOffset));
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        // assignes current distance + 1 if it is lower
        if(cur->getDist()+dist<next->getDist()){
            next->setDist(cur->getDist()+dist);
            next->SetParent(cur);
        }
    }
}

// Draws path by tracing the parent parent pointer
void Pathfinder::drawPath(Cell* start,Cell* end){
    Cell* c = start;
    // Runs until c is at the end
    while(c!=end){
        //Changes color to yellow 
        c->set_fill_color(Color::yellow);
        //sets c to its own parrent
        c=c->getParent();
    }
}

//Returns the lowest distance in the set
Cell* getMinDist(set<Cell*>& q){
    //e is set to the first element of the set
    auto e = *(q.begin());
    //Checks all elements if if has lower distance than e and 
    //sets itself to e
    for(auto c:q){
        if(c->getDist()<e->getDist()){
            e = c;
        }
    }
    return e;
}