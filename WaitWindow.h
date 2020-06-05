#pragma once
#include "Simple_window.h"

//Adaption of Simple_window to include wait with time
struct Wait_window : Simple_window {
	Wait_window(Point xy, int w, int h, const string& title)
		: Simple_window(xy,w,h,title) {}
    

    void wait(double time){
    // Waits for time given in double seconds
        Fl::wait(time);
    }
};
