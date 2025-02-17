#ifndef _SOURCE_VIEW_H
#define _SOURCE_VIEW_H

#include "View.h"

class SourceView : public View {
public:
    explicit SourceView(int x, int y, int width, int height, const char* title);

private:
};

#endif //_SOURCE_VIEW_H