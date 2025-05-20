#ifndef ENGINE_H
#define ENGINE_H

#include "page.h"

class Engine
{
public:
    Engine(Page** pages);
    ~Engine();

private:
    void run();
    void clear();
    void checkForSpecials(char key);
    void exitGame();
    char lowerCase(char key);

private:
    Page** m_pages;
    char m_key;
    short m_currentPage;
    short m_menuIndex;
};

#endif // ENGINE_H
