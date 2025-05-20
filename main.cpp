#include "engine.h"
#include "menupage.h"
#include "howtopage.h"
#include "modepage.h"
#include "newgamepage.h"
#include "constants.h"

#pragma execution_character_set( "utf-8" )

int main()
{
    JsonHandler handler;
    MenuPage menuPage;
    HowtoPage howtoPage;
    ModePage modePage(handler);
    NewGamePage newGamePage(handler);
    Page** pages = new Page*[PAGES_COUNT]; // will be deleted in Engine
    pages[0] = &menuPage;
    pages[1] = &newGamePage;
    pages[2] = &menuPage;
    pages[3] = &howtoPage;
    pages[4] = &modePage;
    Engine engine(pages);

    return 0;
}
