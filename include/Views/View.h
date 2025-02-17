#ifndef _VIEW_H
#define _VIEW_H

#include <string>

#include "FL/Fl.H"
#include "FL/Fl_Window.H"

class View : public Fl_Group {
public:
    explicit View(int x, int y, int width, int height, const char *title);
    void draw() final;
};

#endif
