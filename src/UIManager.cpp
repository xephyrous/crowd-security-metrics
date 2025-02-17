#include "UIManager.h"

#include <stdexcept>
#include <FL/Fl_Button.h>
#include <FL/Fl_Menu_Bar.H>

#include "Compat.h"
#include "Views/InterfaceView.h"
#include "Views/SourceView.h"

/**
 * \brief Initializes the main application window the the set WindowOptions
 */
void UI::Manager::init() {
	// Custom styling
	Fl::scheme("gtk+");
	loadCustomFonts();

	// Initialize window
	m_window = new Fl_Window(
		windowOptions.x, windowOptions.y, 
		windowOptions.width, windowOptions.height, 
		windowOptions.title
	);

    // Instantiate view objects
    m_views[Views::Interface] = new InterfaceView(0, 30, 1200, 570, "Interface View");
    m_views[Views::Sources] = new SourceView(0, 30, 1200, 570, "Sources View");

    m_window->add(m_views[Views::Interface]);

    // Set current view
    m_currentView = Views::Interface;
}

void UI::Manager::switchView(const Views& view) {
	m_views[m_currentView]->hide();
	m_currentView = view;
	m_views[m_currentView]->show();
}

void UI::Manager::run() {
	try {
		if (!m_window) {
			throw std::runtime_error("Main window not initialized, call init() first!");
		}

		// Global menu bar
		Fl_Menu_Bar* menuBar = new Fl_Menu_Bar(0, 0, 1200, 30);
		menuBar->add("File/Open", 0, nullptr, nullptr);
		menuBar->add("View/Interface", 0, []( Fl_Widget* widget, void* data){
            switchView(Views::Interface);
        }, nullptr);
		menuBar->add("View/Sources", 0, []( Fl_Widget* widget, void* data){
            switchView(Views::Sources);
        }, nullptr);

        // Window content
        m_views[m_currentView]->show();

		m_window->end();
		m_window->show();
		Fl::run();
	} catch (const std::exception& e) {
		(void) fprintf(stderr, "Failed to initialize application window!\n%s", e.what());
	}
}

void UI::Manager::openFileDialog(Fl_Widget *w, void *data) {
    // TODO
}

UI::Manager::~Manager() {
	unloadCustomFonts();
}