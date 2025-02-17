#include "Views/InterfaceView.h"
#include "FL/Fl_Button.H"
#include "UIManager.h"
#include "Compat.h"

InterfaceView::InterfaceView(
    int x, int y, int width, int height, const char* title
): View(x, y, width, height, title) {
    try {
        Fl_Button* btn = new Fl_Button(0, 30, 100, 30, "Interface");
        btn->labelfont(fontIndexes["Noto Sans bold"]);
        btn->callback([](Fl_Widget*, void*) {
            //UI::Manager::switchView(UI::Views::Sources);
        }, nullptr);

        this->add(btn);
        this->end();

        this->hide();
    } catch (const std::exception& e) {
        (void) fprintf(stderr, "Failed to initialize Interface View!\n%s", e.what());
    }
}
