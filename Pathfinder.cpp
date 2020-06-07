#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck)
    : Graph_lib::Window{Point{100,100},x*s+70,y*s,"Pathfinder"}, 
    xcell{x},ycell{y},cellSize{s},
    start{start},end{end},blocked{blck},
    running{false},startPress{false},endPress{false},
    dijkstraBt{Point{x_max()-70,0},70,20,"Dijkstra",cb_start},
    clearBt{Point{x_max()-70,25},70,20,"Clear",cb_clear}
    {
    //Fills up the grid and the que
    for (int j = 0;j<xcell;j++){
        for (int i = 0;i<ycell;i++){
            vr.push_back(new Cell{Point{i*cellSize,j*cellSize},cellSize,cellSize,Loc{i,j}});
            attach(vr.back());
            q.insert(&vr.back());
        }
    }

    attach(dijkstraBt);
    attach(clearBt);

    //Color start and end
    getCell(start)->set_fill_color(Color::magenta);
    getCell(end)->set_fill_color(Color::green);
    //Blocks all cells from the blck vector
    for(auto b:blocked){
        getCell(b)->setBlocked();
    }

    // Opens window
    show();

    srand((unsigned) time(0));
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
    catch (range_error& e){
        //std::cerr<<e.what();
        return nullptr;
    }
}

void Pathfinder::dijkstra(){ 
    // the current cell is set to the start location
    auto cur = getCell(start);
    //curs distance is set to 0. The rest are infinite from the constructor
    cur->setDist(0);


    //Runs until the end cell has been visited
    while(getCell(end)->getStatus()!=Stat::visited){
        //The current cell is set to the cell with the lowest distance in q
        cur = getMinDist();

        //checks all directions
        //compareCells(cur,-1,-1);// up left
        compareCells(cur,0,-1);// up
        //compareCells(cur,1,-1);// up right
        compareCells(cur,1,0); // right
        //compareCells(cur,1,1); // down right
        compareCells(cur,0,1); // down
        //compareCells(cur,-1,1); // down left
        compareCells(cur,-1,0);// left

        // Done with current so its set to visited and removed from q
        cur->setVisited();
        searched.push_back(cur);
        q.erase(cur);
        getCell(start)->set_fill_color(Color::magenta);
        flush(); //redraws window
        Fl::wait(0.1); //waits for 0.1 seconds
    }
    //draws path from end to start
    drawPath(cur,getCell(start));
    getCell(end)->set_fill_color(Color::green);
    flush();
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

//Handles click events while not running
void Pathfinder::handleClicks(){
    while(!running){
        Fl::wait();
        click();         
    }
}


// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCells(Cell* cur,int xOffset,int yOffset){
    //double dist =  sqrt(abs(xOffset)+abs(yOffset));
    double dist =  abs(xOffset)+abs(yOffset);
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        next->set_fill_color(Color::blue);
        searched.push_back(next);
        // assignes current distance + cost if it is lower
        if(cur->getDist()+dist<next->getDist()){
            next->setDist(cur->getDist()+dist);
            next->SetParent(cur);
        }
    }
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

void Pathfinder::click(){
    Point xy{Fl::event_x(),Fl::event_y()};//Mouse position
    MouseButton mb = static_cast<MouseButton>(Fl::event_button()); // Mouse button

    // If a mouse button is pressed and is inside grid
    if(Fl::event_buttons()>0 && inRange(xy)){
        // c is set to pointer to cell under mouse
        auto c = getCell(pntToLoc(xy));

        switch (mb)
        {
        case MouseButton::left:
            //sets startPress and endPress to true for dragging
            if(c->getLoc() == start){
                startPress = true;
            }
            else if(c->getLoc() == end){
                endPress = true;
            }
            // If startPress or endPress is true, it moves the start or end to 
            // the new location of the mouse
            else if(startPress){
                setStart(c->getLoc());
            }
            else if(endPress){
                setEnd(c->getLoc());
            }
            // Blocks cell under mouse
            else{
                c->setBlocked();
            }
            break;
        case MouseButton::right:
            // Empties cell under mouse
            c->setEmpty();
            break;
        }
        // reloads window
        flush();
    }
    else{
        //Stops dragging
        startPress = false;
        endPress = false;
    }
}

//Changes start cell to the given location
void Pathfinder::setStart(Loc l){
    getCell(start)->setEmpty();
    start=l;
    getCell(start)->set_fill_color(Color::magenta);
}

//Changes end cell to the given location
void Pathfinder::setEnd(Loc l){
    getCell(end)->setEmpty();
    end=l;
    getCell(end)->set_fill_color(Color::green);
}

// Callback function from the start button
void Pathfinder::cb_start(Address, Address addr){ 
    static_cast<Pathfinder *>(addr)->strt();
}

//starts the algorithm 
void Pathfinder::strt() {
    running = true;
    dijkstra();
    running = false;
    // runs handleClicks again to keep window open after completion
    handleClicks();
}

void Pathfinder::cb_clear(Address, Address addr){ 
    static_cast<Pathfinder *>(addr)->clear();
}

void Pathfinder::clear() {
    //Set all searched cells to empty
    for(auto c:searched){
        c->setEmpty();
    }
    searched.clear();
    //redraw window
    flush();
    //Color start and end
    getCell(start)->set_fill_color(Color::magenta);
    getCell(end)->set_fill_color(Color::green);

    q.clear();
    for(auto e:vr){
        e->setDist(std::numeric_limits<double>::infinity());
        q.insert(e);
    };

}

void Pathfinder::mazeGen(){
    for(auto c:vr){
        c->setBlocked();
    }
    show();
    setEnd(Loc(xcell-1,ycell-1));

    Cell* nxt;
    stack<Cell*> st;
    vector<Cell*> nb;
    st.push(getCell(start));
    st.top()->setEmpty();
    Cell* cur;

    while(st.size()>0){
        cur = st.top();
        st.pop();
        vector<pair<int,int>> dir{make_pair(0,1),make_pair(1,0),
                                  make_pair(0,-1),make_pair(-1,0)};

        do{
            int i = rand() % dir.size();
            auto d = dir[i];
            dir.erase(dir.begin()+i);
            nxt = openCell(cur,d.first,d.second);
        }
        while(nxt == nullptr && dir.size()>0 );
        if(nxt!= nullptr){
            //cur->set_fill_color(Color::gray);
            //nxt->set_fill_color(Color::dark_yellow);
            st.push(cur);
            st.push(nxt);
            Fl::wait(.25);
            flush();
        }
    }
}

Cell* Pathfinder::openCell(Cell* cur,int xOffset,int yOffset){
    auto wall = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(wall!=nullptr && wall->getStatus()==Stat::blocked){
        auto next = getCell(Loc{wall->getLoc().x+xOffset,wall->getLoc().y+yOffset});
        if(next!=nullptr && next->getStatus()==Stat::blocked){
            //next->set_fill_color(Color::blue);
            wall->setEmpty();
            next->setEmpty();
            return next;
        }
        else{
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}



//Cell* Pathfinder::checkCell(Cell* cur,int xOffset,int yOffset){
//    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
//    // Checks if cell is inside grid and is empty
//    if(next!=nullptr && next->getStatus()==Stat::blocked){
//        return next;
//    }
//}