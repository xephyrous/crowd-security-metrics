#pragma once

#include <string>
#include <unordered_map>

#include "FL/Fl.H"
#include "FL/Fl_Window.H"

namespace UI {
	typedef struct {
		int x;
		int y;
		int width;
		int height;
		const char* title;
	} WindowOptions;

	enum Views {
		Interface,
		Sources,
		Count // Sentinel value for counting views
	};

	class Manager {
	public:
		Manager() = delete;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
		~Manager();

		static void init();
		static void run();
		static void switchView(const Views& view);

		static inline WindowOptions windowOptions = WindowOptions {
			0, 0, 800, 600, "FLTK Window"
		};

	private:
		static inline Fl_Window* m_window = nullptr;
		static inline Fl_Group* m_views[Views::Count];
		static inline Views m_currentView = Views::Interface;

		static void initInterfaceView();
		static void initSourcesView();

		void openFileDialog(Fl_Widget* w, void* data);
	};

}
