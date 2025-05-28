#include "engine.h"
#include "menupage.h"
#include "howtopage.h"
#include "modepage.h"
#include "newgamepage.h"
#include "loadgamepage.h"
#include "recordpage.h"
#include "constants.h"

#pragma execution_character_set( "utf-8" )

int main()
{
    JsonHandler handler;
    MenuPage menuPage;
    HowtoPage howtoPage;
    ModePage modePage(handler);
    NewGamePage newGamePage(handler);
    LoadGamePage loadGamePage(handler);
    RecordPage recordPage(handler);

    Page** pages = new Page*[PAGES_COUNT]; // will be deleted in Engine
    pages[0] = &menuPage;
    pages[1] = &newGamePage;
    pages[2] = &loadGamePage;
    pages[3] = &recordPage;
    pages[4] = &howtoPage;
    pages[5] = &modePage;
    Engine engine(pages);

    return 0;
}
