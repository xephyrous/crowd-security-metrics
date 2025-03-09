#ifndef _COMPAT_H
#define _COMPAT_H

#include <cstdio>
#include <string>
#include <filesystem>
#include <vector>

#include "FL/Fl.H"

/**
 * \brief A mapping of loaded custom font names to their indexes in FLTK
 * \note This uses the system font names, not the file names!
 */
static std::unordered_map<std::string, int> fontIndexes;
static std::vector<std::string> customFonts;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define NOMINMAX
#include <windows.h>

/**
 * \brief Loads all fonts in Assets/Fonts to memory and maps their names to indexes in FLTK
 */
inline void loadCustomFonts() {
    const std::string directory = "Assets\\Fonts";

    if (!std::filesystem::exists(directory)) {
        (void) fprintf(stderr, "Fonts directory '%s' does not exist", directory.c_str());
        return;
    }

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile((directory + "\\*.ttf").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        (void) fprintf(stderr, "Failed to open directory or no .ttf files found!");
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string fontPath = directory + "\\" + findFileData.cFileName;

            const int res = AddFontResourceEx(fontPath.c_str(), FR_PRIVATE, NULL);
            if (res == 0) {
                (void) fprintf(stderr, "Failed to load font '%s'!\n", fontPath.c_str());
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Map custom font names to indexes in FLTK
    std::string name;
    for (int i = Fl::set_fonts() - 1; i > 0; i--) {
	    if ((name = std::string(Fl::get_font_name(i))).find("Noto Sans") == 0) {
            fontIndexes.insert(std::make_pair(name, i));
            customFonts.emplace_back(name);

            // TODO : Move to logger
            printf("Successfully loaded font %-50s [%d]\n", name.c_str(), i);
	    }
    }
}

inline void unloadCustomFonts() {
    const std::string directory = "Assets\\Fonts";

    if (!std::filesystem::exists(directory)) {
        (void)fprintf(stderr, "Fonts directory '%s' does not exist\n", directory.c_str());
        return;
    }

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile((directory + "\\*.ttf").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        (void)fprintf(stderr, "Failed to open directory or no .ttf files found!\n");
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string fontPath = directory + "\\" + findFileData.cFileName;

            const int res = RemoveFontResourceEx(fontPath.c_str(), FR_PRIVATE, NULL);
            if (res == 0) {
                (void)fprintf(stderr, "Failed to unload font %s!\n", fontPath.c_str());
            } else {
                printf("Successfully unloaded font %s.\n", fontPath.c_str());
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}
#endif // WIN32, CYGWIN

#define __unix__
#if defined(__unix__)

// TODO : UNIX stuff

#endif // unix
#endif // _COMPAT_H