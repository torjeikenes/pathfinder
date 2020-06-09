#include "Cell.h"


void Cell::setEmpty(){
    status = Stat::empty;
    set_fill_color(emptyC);
    
}

void Cell::setBlocked(){
    status = Stat::blocked;
    set_fill_color(blockC);
}

void Cell::setVisited(){
    status = Stat::visited;
    set_fill_color(visitedC);
}

bool Cell::operator<(Cell* rhs){
    return this->getCost()<rhs->getCost();
}

bool Cell::operator>(Cell* rhs){
    return this->getCost()>rhs->getCost();
}