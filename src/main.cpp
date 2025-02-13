#include <UIManager.h>
#include <iostream>

int main(int argc, char **argv) {
    UI::Manager::windowOptions = UI::WindowOptions{ 0, 0, 1200, 600, "Crowd Security Metrics v0.1a" };
    UI::Manager::init();
    UI::Manager::run();
    return 0;
}