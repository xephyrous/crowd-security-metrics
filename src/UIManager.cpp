#include "UIManager.h"

#include <stdexcept>
#include <FL/Fl_Button.h>
#include <FL/Fl_Menu_Bar.H>

#include "Compat.h"

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

	// Initialize views
	initInterfaceView();
	initSourcesView();

	m_views[Views::Interface]->show();
}

void UI::Manager::initInterfaceView() {
	m_views[Views::Interface] = new Fl_Group(0, 30, 1200, 570, "Interface View");

	Fl_Button* btn = new Fl_Button(0, 30, 50, 50, "A");
	btn->labelfont(fontIndexes["Noto Sans bold"]);
	btn->callback([](Fl_Widget*, void* data) {
		UI::Manager::switchView(Views::Sources);
		}, nullptr);

	m_views[Views::Interface]->end();
	m_views[Views::Interface]->hide();
}

void UI::Manager::initSourcesView() {
	m_views[Views::Sources] = new Fl_Group(0, 30, 1200, 570, "Sources View");

	Fl_Button* btn = new Fl_Button(0, 30, 50, 50, "B");
	btn->labelfont(fontIndexes["Noto Sans bold"]);
	btn->callback([](Fl_Widget*, void* data) {
		UI::Manager::switchView(Views::Interface);
	}, nullptr);

	m_views[Views::Sources]->end();
	m_views[Views::Sources]->hide();
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
		menuBar->add("View/Interface", 0, nullptr, nullptr);
		menuBar->add("View/Sources", 0, nullptr, nullptr);

		m_window->end();
		m_window->show();
		Fl::run();
	} catch (const std::exception& e) {
		(void) fprintf(stderr, "Failed to initialize application window!\n%s", e.what());
	}
}

UI::Manager::~Manager() {
	unloadCustomFonts();
}
