#include "Cell.h"


void Cell::setEmpty(){
    status = Stat::empty;
    set_fill_color(Color::gray);
}

void Cell::setBlocked(){
    status = Stat::blocked;
    set_fill_color(Color::black);
}

void Cell::setVisited(){
    status = Stat::visited;
    set_fill_color(Color::cyan);
}
