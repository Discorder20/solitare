#ifndef LOADGAMEPAGE_H
#define LOADGAMEPAGE_H

#include "game.h"
#include "page.h"

class LoadGamePage : public Page
{
public:
    LoadGamePage(JsonHandler &handler);
    ~LoadGamePage();

    void render() override;
    void handleKey(char key) override;
    void start() override;

private:
    Game m_game;
    JsonHandler &m_handler;
};

#endif // LOADGAMEPAGE_H
