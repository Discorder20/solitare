#ifndef NEWGAMEPAGE_H
#define NEWGAMEPAGE_H

#include "game.h"
#include "page.h"

class NewGamePage : public Page
{
public:
    NewGamePage(JsonHandler &handler);
    ~NewGamePage();

    void render() override;
    void handleKey(char key) override;
    void start() override;

private:
    Game m_game;
};

#endif // NEWGAMEPAGE_H
