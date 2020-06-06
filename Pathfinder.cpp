#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck)
    : xcell{x},ycell{y},cellSize{s},
    Graph_lib::Window{Point{100,100},x*s+100,y*s+50,"Pathfinder"}, 
    //Graph_lib::Window{Point{100,100},400,400,"Pathfinder"}, 
    start{start},end{end},blocked{blck},
    startbt{Point{x_max()-70,0},70,20,"Start",cb_start},
    running{false}
    {
    //Fills up the grid
    //fillGrid();
    for (int j = 0;j<xcell;j++){
        for (int i = 0;i<ycell;i++){
            vr.push_back(new Cell{Point{i*cellSize,j*cellSize},cellSize,cellSize,Loc{i,j}});
            //attach(vr[vr.size()-1]);
            attach(vr.back());
            q.insert(&vr.back());
        }
    }


    attach(startbt);

    //Color start and end
    getCell(start)->set_fill_color(Color::magenta);
    getCell(end)->set_fill_color(Color::green);
    //Blocks all cells from the blck vector
    for(auto b:blocked){
        getCell(b)->setBlocked();
    }
    //resizable(nullptr);
    //size_range(x_max(),y_max(),x_max(),y_max());

    show();
    Fl::wait(1);
}

//Fills grid with cells and attaches to vr
//void Pathfinder::fillGrid(){
//}

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
    //set<Cell*> q;
    //for(auto e:vr){
    //    q.insert(e);
    //};

    //start window and wait 1 sec


    //Runs until the end cell has been visited
    while(getCell(end)->getStatus()!=Stat::visited){
        //The current cell is set to the cell with the lowest distance in q
        cur = getMinDist();

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
        flush();
        //waits for 0.25 seconds
        Fl::wait(0.15); 
        //Sleep(2000);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //win.wait_for_button();
    }
    //draws path from end to start
    drawPath(cur,getCell(start));
    getCell(end)->set_fill_color(Color::green);
    flush();
    
    //win.wait_for_button();
}

// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCells(Cell* cur,int xOffset,int yOffset){
    //double dist =  sqrt(abs(xOffset)+abs(yOffset));
    double dist =  abs(xOffset)+abs(yOffset);
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        // assignes current distance + cost if it is lower
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

void Pathfinder::drawBlocked(){
    while(!running){
        Fl::wait();
        click();         
    }
}


void Pathfinder::click(){
    Point xy{Fl::event_x(),Fl::event_y()};
    MouseButton mb = static_cast<MouseButton>(Fl::event_button());


    if(Fl::event_buttons()>0){
        switch (mb)
        {
        case MouseButton::left:
            if(inRange(xy))
                getCell(pntToLoc(xy))->setBlocked();
            break;
        case MouseButton::right:
            if(inRange(xy))
                getCell(pntToLoc(xy))->setEmpty();
            break;
        }
        flush();
    }
}

void Pathfinder::cb_start(Address, Address addr){ 
    static_cast<Pathfinder *>(addr)->strt();
}

void Pathfinder::strt() {
    running = true;
    dijkstra();
    running = false;
    drawBlocked();
}

//Returns the lowest distance in the set
Cell* Pathfinder::getMinDist(){
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


//void Pathfinder::cb_click(Address, Address pw){
//    Point xy{Fl::event_x(),Fl::event_y()};
//    MouseButton mb = static_cast<MouseButton>(Fl::event_button());
//    auto& win = reference_to<Pathfinder>(pw);
//
//    if(!win.inRange(xy)){
//        return;
//    }
//    switch (mb)
//    {
//    case MouseButton::left:
//        win.getCell(win.pntToLoc(xy))->setBlocked();;
//        break;
//    case MouseButton::right:
//        win.getCell(win.pntToLoc(xy))->setEmpty();
//        break;
//    }
//    win.flush();
//}
//