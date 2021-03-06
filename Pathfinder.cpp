#include "Pathfinder.h"

Pathfinder::Pathfinder(int x, int y, int s,Loc start, Loc end,Vector<Loc> blck)
    : Graph_lib::Window{Point{100,100},x*s+70,y*s,"Pathfinder"}, 
    dijkstraBt{Point{x_max()-70,0},70,20,"Dijkstra",cb_start},
    AstBt{Point{x_max()-70,25},70,20,"A*",cb_ast},
    mazeBt{Point{x_max()-70,50},70,20,"Maze",cb_maze},
    clearBt{Point{x_max()-70,75},70,20,"Clear",cb_clear},
    moves{Point{x_max()-70,114},"Moves: 0"},
    mvBg{Point{x_max()-70,100},70,20},
    xcell{x},ycell{y},cellSize{s},moveCtr{0},
    blocked{blck},start{start},end{end},
    running{false},startPress{false},endPress{false},dijkDone{false},astDone{false},
    searchC{Color::blue},pathC{Color::yellow},startC{Color::magenta},
    endC{Color::green}
    {
    //Fills up the grid and the que
    for (int j = 0;j<xcell;j++){
        for (int i = 0;i<ycell;i++){
            vr.push_back(new Cell{Point{i*cellSize,j*cellSize},
                                        cellSize,cellSize,Loc{i,j},this});
            attach(vr.back());
            //q.insert(&vr.back());
        }
    }

    moves.set_color(Color::black);
    mvBg.set_fill_color(Color::gray);

    attach(dijkstraBt);
    attach(AstBt);
    attach(clearBt);
    attach(mazeBt);
    attach(mvBg);
    attach(moves);

    //Color start and end
    getCell(start)->set_fill_color(startC);
    getCell(end)->set_fill_color(endC);
    //Blocks all cells from the blck vector
    for(auto b:blocked){
        getCell(b)->setBlocked();
    }

    // Opens window
    show();

    srand((unsigned) time(0));
}



#pragma region dijkstra

void Pathfinder::dijkstra(double waitTime=0){ 
    running = true;
    clear();
    // the current cell is set to the start location
    auto cur = getCell(start);
    //curs distance is set to 0. The rest are infinite from the constructor
    cur->setDist(0);
    cur->setCost(cur->getDist());
    q.push(cur);
    



    //Runs until the end cell has been visited
    while(getCell(end)->getStatus()!=Stat::visited){
        //The current cell is set to the cell with the lowest distance in q
        cur = getMinDist();
        addMove();

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
        //q.erase(cur);
        getCell(start)->set_fill_color(startC);
        if(waitTime>0){
            flush(); //redraws window
            Fl::wait(waitTime); //waits for 0.1 seconds
        }
    }
    //draws path from end to start
    drawPath(cur,getCell(start));
    getCell(end)->set_fill_color(endC);
    flush();
    running = false;
    dijkDone = true;
}

// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCells(Cell* cur,int xOffset,int yOffset){
    int d = 10; // multiplier for minimum cost
    int dist =  sqrt(abs(xOffset*d)+abs(yOffset*d));
    //int dist =  abs(xOffset*10)+abs(yOffset*10);
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        next->set_fill_color(searchC);
        searched.push_back(next);
        // assignes current distance + cost if it is lower
        if(cur->getDist() + dist < next->getDist()){
            next->setDist(cur->getDist() + dist);
            next->SetParent(cur);
            q.push(next);
        }
    }
}

//Returns the lowest distance in the set
Cell* Pathfinder::getMinDist(){
    auto e = q.top();
    q.pop();
    //e is set to the first element of the set
    //auto e = *(q.begin());
    ////Checks all elements if if has lower distance than e and 
    ////sets itself to e
    //for(auto c:q){
    //    if(c->getDist()<e->getDist()){
    //        e = c;
    //    }
    //}
    return e;
}

#pragma endregion

#pragma region a-star
void Pathfinder::aStar(double waitTime=0){ 
    running = true;
    clear();
    // the current cell is set to the start location
    auto cur = getCell(start);
    //curs distance is set to 0. The rest are infinite from the constructor
    cur->setDist(0);
    cur->setCost(0+manhattan(cur));
    q.push(cur);


    //Runs until the end cell has been visited
    while(getCell(end)->getStatus()!=Stat::visited){
        //The current cell is set to the cell with the lowest distance in q
        cur = getMinCost();
        addMove();

        //checks all directions
        //compareCellsAst(cur,-1,-1);// up left
        compareCellsAst(cur,0,-1);// up
        //compareCellsAst(cur,1,-1);// up right
        compareCellsAst(cur,1,0); // right
        //compareCellsAst(cur,1,1); // down right
        compareCellsAst(cur,0,1); // down
        //compareCellsAst(cur,-1,1); // down left
        compareCellsAst(cur,-1,0);// left

        // Done with current so its set to visited and removed from q
        cur->setVisited();
        searched.push_back(cur);
        //q.erase(cur);
        getCell(start)->set_fill_color(startC);
        if(waitTime>0){
            flush(); //redraws window
            Fl::wait(waitTime); //waits for 0.1 seconds
        }
    }
    //draws path from end to start
    drawPath(cur,getCell(start));
    getCell(end)->set_fill_color(endC);
    flush();
    running = false;
    astDone = true;
}

//Returns manhattan distance between cell and end
int Pathfinder::manhattan(Cell* c){
    int d = 10; // multiplier for minimum cost
    Loc cxy = c->getLoc();

    int dx = abs(cxy.x-end.x);
    int dy = abs(cxy.y-end.y);
    return d*(dx + dy);
}

//Returns the lowest cost in the set
Cell* Pathfinder::getMinCost(){
    auto e = q.top();
    q.pop();
    //e is set to the first element of the set
    //auto e = *(q.begin());
    ////Checks all elements if if has lower cost than e and 
    ////sets itself to e
    //for(auto c:q){
    //    if(c->getCost() < e->getCost()){
    //        e = c;
    //    }
    //    // If the cost is the same, the shortest manhattan distance is prioritized 
    //    else if((c->getCost()==e->getCost()) && (manhattan(c)< manhattan(e))){
    //        e = c;
    //    }
    //}
    return e;
}

// Compares current cell with the cell with given offset and set distance
void Pathfinder::compareCellsAst(Cell* cur,int xOffset,int yOffset){
    int d = 10; // multiplier for minimum cost
    int mvCost =  abs(xOffset*d)+abs(yOffset*d);

    //int dist =  abs(xOffset*10)+abs(yOffset*10);
    auto next = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if cell is inside grid and is empty
    if(next!=nullptr && next->getStatus()==Stat::empty){
        next->set_fill_color(searchC);
        int manh = manhattan(next);
        searched.push_back(next);
        // assignes current distance + cost if it is lower
        if(cur->getCost() + mvCost < next->getCost()){
            next->setDist(cur->getDist() + mvCost);
            next->setCost(next->getDist() + manh);
            next->SetParent(cur);
            q.push(next);
        }
    }
}

#pragma endregion

// Draws path by tracing the parent pointer
void Pathfinder::drawPath(Cell* start,Cell* end){
    Cell* c = start;
    // Runs until c is at the end
    while(c!=end){
        //Changes color to yellow 
        c->set_fill_color(pathC);
        //sets c to its own parrent
        c=c->getParent();
    }
}

#pragma region maze
void Pathfinder::mazeGen(double waitTime=0){
    clear();
    Loc last = end;
    setEnd(Loc(xcell-1,ycell-1));

    // Set all cells to blocked
    for(auto c:vr){
        c->setBlocked();
    }

    stack<Cell*> st; // stack for visited cells
    Cell* nxt; // Next cell
    Cell* cur; // Current cell

    st.push(getCell(start)); //Add first cell to the que stack
    st.top()->setEmpty(); // set the first cell to empty
    flush(); // Refresh window

    while(st.size()>0){
        cur = st.top(); // set the current cell to the cell of stack
        st.pop(); // and delete it from the stack

        // Vector with directions
        vector<pair<int,int>> dir{make_pair(0,1),make_pair(1,0),
                                  make_pair(0,-1),make_pair(-1,0)};

        do{
            // Choose a random direction and delete it from the direction vector
            int i = rand() % dir.size(); 
            auto d = dir[i];
            dir.erase(dir.begin()+i);

            // Try to open wall and cell in the chosen direction
            nxt = openCell(cur,d.first,d.second);
        }
        // Runs until a direction is inside grid or no direction is possible
        while(nxt == nullptr && dir.size()>0 ); 

        // If a direction is possible
        if(nxt != nullptr){
            //add current and next position to the stack
            st.push(cur);
            st.push(nxt);

            Fl::wait(waitTime);// wait

            // Set cur cell to last cell 
            last = nxt->getLoc();
            flush(); 
        }
    }
    // Set start and end to make them visible
    setStart(start);
    setEnd(last);
    flush();
}

// Open wall and cell in given direction or return nullptr if not possible
Cell* Pathfinder::openCell(Cell* cur,int xOffset,int yOffset){
    // Wall cell is the cell next to the current cell in the given direction
    auto wall = getCell(Loc{cur->getLoc().x+xOffset,cur->getLoc().y+yOffset});
    // Checks if wall is inside grid and is blocked
    if(wall!=nullptr && wall->getStatus()==Stat::blocked){
        // Next cell is the cell next to the wall cell in the given direction
        auto next = getCell(Loc{wall->getLoc().x+xOffset,wall->getLoc().y+yOffset});
        // Checks if next cell is inside grid and is blocked
        if(next!=nullptr && next->getStatus()==Stat::blocked){
            // set wall and cell to empty 
            wall->setEmpty();
            next->setEmpty();
            return next;
        }
        else{
            // Return nullptr if cell is outside grid or not blocked
            return nullptr; 
        }
    }
    else
    {
        // Return nullptr if wall is outside grid or not blocked
        return nullptr;
    }
}
#pragma endregion

#pragma region clicks
//Handles click events while not running
void Pathfinder::handleClicks(){
    while(!running){
        Fl::wait();
        click();         
    }
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
        //if(startPress || endPress){
        //    if(dijkDone){
        //        dijkstra();
        //    }
        //    else if(astDone){
        //        aStar();
        //    }
        //}
        //Stops dragging
        startPress = false;
        endPress = false;
    }
}

#pragma endregion

#pragma region set-get
//Changes start cell to the given location
void Pathfinder::setStart(Loc l){
    getCell(start)->setEmpty();
    start=l;
    getCell(start)->set_fill_color(startC);
    if(dijkDone){
        dijkstra();
    }
    else if(astDone){
        aStar();
    }
}

//Changes end cell to the given location
void Pathfinder::setEnd(Loc l){
    getCell(end)->setEmpty();
    end=l;
    getCell(end)->set_fill_color(endC);
    if(dijkDone){
        dijkstra();
    }
    else if(astDone){
        aStar();
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
    catch (range_error& e){
        //std::cerr<<e.what();
        return nullptr;
    }
}

#pragma endregion

#pragma region callback
// Maze button callback
void Pathfinder::cb_maze(Address, Address addr){
    static_cast<Pathfinder *>(addr)->mazeGen(0.05);
}

// Astar button callback
void Pathfinder::cb_ast(Address, Address addr){
    static_cast<Pathfinder *>(addr)->aStar(0.05);
}

// Callback function from the start button
void Pathfinder::cb_start(Address, Address addr){ 
    static_cast<Pathfinder *>(addr)->dijkstra(0.05);
}

void Pathfinder::cb_clear(Address, Address addr){ 
    static_cast<Pathfinder *>(addr)->clearBlk();
}
#pragma endregion

// Increment the move counter
void Pathfinder::addMove(){
    moveCtr++;
    moves.set_label("Moves: " + to_string(moveCtr));
}

#pragma region clear
// Clear all searched cells. Not blocked
void Pathfinder::clear() {
    //Set all searched cells to empty
    for(auto c:searched){
        c->setEmpty();
    }
    searched.clear();
    //Color start and end
    getCell(start)->set_fill_color(startC);
    getCell(end)->set_fill_color(endC);

    //q.clear();
    while(!q.empty()){
        q.pop();
    }

    for(auto e:vr){
        e->setDist(std::numeric_limits<int>::max());
        e->setCost(std::numeric_limits<int>::max());
        //q.insert(e);
    };
    moveCtr = 0;
    moves.set_label("Moves: 0");

    //redraw window
    flush();
}
// Clear all cells including blocked
void Pathfinder::clearBlk(){
    //Set all searched cells to empty
    for(auto c:vr){
        c->setEmpty();
    }
    searched.clear();
    //Color start and end
    getCell(start)->set_fill_color(startC);
    getCell(end)->set_fill_color(endC);

    //q.clear();
    while(!q.empty()){
        q.pop();
    }
    for(auto e:vr){
        e->setDist(std::numeric_limits<int>::max());
        e->setCost(std::numeric_limits<int>::max());
        //q.insert(e);
    };
    moveCtr = 0;
    moves.set_label("Moves: 0");

    dijkDone = false;
    astDone = false;

    //redraw window
    flush();
}
#pragma endregion

// Cell comparator to sort the priority_queue
bool cellCompareGt::operator()(Cell* lhs, Cell* rhs){
    auto pf = lhs->getPf(); // pointer to the pathfinder to use manhattan
    if (lhs->getCost() > rhs->getCost()){
        return true;
    }
    else if (lhs->getCost() == rhs->getCost()){
        return pf->manhattan(lhs) > pf->manhattan(rhs);
    }
    else{
        return false;
    }
}