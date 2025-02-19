#include <UIManager.h>
#include <MaskRCNN.h>
#include <iostream>


int main(int argc, char **argv) {

     MaskRCNN model("../model/mask_rcnn.pt");

    // UI
    UI::Manager::windowOptions = UI::WindowOptions{ 0, 0, 1200, 600, "Crowd Security Metrics v0.1a" };
    UI::Manager::init();
    UI::Manager::run();
    return 0;
}