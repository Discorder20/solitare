#include "engine.h"
#include "constants.h"
#include "windows.h" // for consoleCP

#include <conio.h>

using namespace std;

Engine::Engine(Page**pages)
    : m_pages(pages)
    , m_currentPage(0)
    , m_menuIndex(0)
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    run();
}

Engine::~Engine()
{
    for (short i = 0; i < PAGES_COUNT; ++i) {
        delete m_pages[i];
    }
    delete [] m_pages;
}

void Engine::run()
{
    while (true) {
        clear();
        m_pages[m_currentPage]->render();
        m_key = lowerCase(_getch());
        m_pages[m_currentPage]->handleKey(m_key);
        checkForSpecials(m_key);
    }
}

void Engine::clear()
{
    system("cls");
}

void Engine::checkForSpecials(char key)
{
    if (m_currentPage == 0) { // if is in menu
        if (key == 'w') {
            m_menuIndex = m_menuIndex > 0 ? m_menuIndex - 1 : 0;
        } else if (key == 's') {
            m_menuIndex = m_menuIndex < (PAGES_COUNT - 2) ? m_menuIndex + 1 : PAGES_COUNT - 2; // PAGES - 1 because first page is menu page
        } else if (key == ENTER_KEY) {
            if (m_menuIndex == PAGES_COUNT - 2) { // if the user will select "wyjdź z gry"
                exitGame();
            } else {
                m_currentPage = m_menuIndex + 1;
                m_pages[m_currentPage]->start();
            }
        }
    } else if (key == ESCAPE_KEY) {
        m_currentPage = 0; // we go back to menu
    }
}

void Engine::exitGame()
{
    delete [] m_pages;
    exit(0);
}

char Engine::lowerCase(char key)
{
    if (key >= 65 && key <= 90) {
        return key + 32;
    }
    return key;
}
