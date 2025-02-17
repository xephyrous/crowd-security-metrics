#ifndef _UI_MANAGER_H
#define _UI_MANAGER_H

#include <string>
#include <unordered_map>

#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "Views/View.h"

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
		Sources
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
        static inline Views m_currentView = Views::Interface;
        static inline std::unordered_map<Views, View*> m_views;

		void openFileDialog(Fl_Widget* w, void* data);
	};

}

#endif // _UI_MANAGER_H