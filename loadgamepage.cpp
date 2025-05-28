#include "loadgamepage.h"
#include "constants.h"

#include "colors.h"

LoadGamePage::LoadGamePage(JsonHandler &handler)
    : m_game(handler)
    , m_handler(handler)
{
}

LoadGamePage::~LoadGamePage()
{
}

void LoadGamePage::render()
{
    if (m_game.isGameWon()) {
        printColoredLine(COLOR_LIGHT_GREEN, "Wygrano grę !!!");
        printColoredLine(COLOR_LIGHT_GREEN, "Liczba ruchów: " + std::to_string(m_handler.getMoves()));
        printColoredLine(COLOR_WHITE, "Naciśnij przycisk Esc aby wrócić do menu");
    } else {
        printColoredLine(COLOR_WHITE, "Naciśnij Esc aby opuścić grę (Zapis odbywa się automatycznie): ");
        printColoredLine(COLOR_LIGHT_YELLOW, "Liczba ruchów: " + std::to_string(m_handler.getMoves()) + "\n");
        printColoredLine(COLOR_LIGHT_YELLOW, "Dostępne cofnięcia: " + std::to_string(m_game.getCurrentStep()) + "\n");
        for (short i = 0; i < m_game.rows(); ++i) {
            m_game.printRow(i);
        }
    }
}

void LoadGamePage::handleKey(char key)
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
    } else if (key == 'c') {
        m_game.cancelSelection();
    } else if (key == 'z') {
        m_game.cancelMove();
    }
}

void LoadGamePage::start()
{
    m_game.loadGame();
}
