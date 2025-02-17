#include "Views/View.h"

View::View(int x, int y, int width, int height, const char *title) :
    Fl_Group(x, y, width, height, title) { }

void View::draw() {
    Fl_Group::draw();
}