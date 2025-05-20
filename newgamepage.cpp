#include "newgamepage.h"
#include "constants.h"

NewGamePage::NewGamePage(JsonHandler &handler)
    : m_game(handler)
{
}

NewGamePage::~NewGamePage()
{
}

void NewGamePage::render()
{
    for (short i = 0; i < m_game.rows(); ++i) {
        m_game.printRow(i);
    }
}

void NewGamePage::handleKey(char key)
{
    if (key == 'w') {
        m_game.moveTop();
    } else if (key == 's') {
        m_game.moveBottom();
    } else if (key == 'd') {
        m_game.moveRight();
    } else if (key == 'a') {
        m_game.moveLeft();
    } else if (key == ENTER_KEY) {
        m_game.handleClick();
    }
}

void NewGamePage::start()
{
    m_game.initializeGame();
}
