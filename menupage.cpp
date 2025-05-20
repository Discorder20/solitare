#include "menupage.h"
#include "card.h"
#include "constants.h"
#include "colors.h"

#include <iostream>

using namespace std;

MenuPage::MenuPage()
    : m_index(0)
{
}

MenuPage::~MenuPage()
{
}

void MenuPage::render()
{
    const string options[] = {"uruchom nową grę", "wczytaj ostatnią grę", "sterowanie", "ustawienia", "wyjdź z gry"};

    cout << "Witaj w grze Pasjans\n" << endl;
    cout << "Poruszaj się po tym menu wykorzystując przyciski 'w','s' oraz 'Enter' \n" << endl;

    for (short i = 0; i < PAGES_COUNT - 1; ++i) {
        short color = m_index == i ? COLOR_CYAN : COLOR_WHITE;
        printColoredLine(color, options[i]);
    }

    Card card(7 + m_index, m_index % 4, false); // card icon, design part
    for (int i = 0; i < CARD_HEIGHT; ++i) {
        printColoredLine(m_index + 10, card.getRow(i));
    }

    cout << "\nAutor: Michał Borzuchowski" << endl;
}

void MenuPage::handleKey(char key)
{
    if (key == 'w') {
        m_index = m_index > 0 ? m_index - 1 : 0;
    } else if (key == 's') {
        m_index = m_index < (PAGES_COUNT - 2) ? m_index + 1 : PAGES_COUNT - 2; // PAGES - 1 because first page is menu page
    }
}

void MenuPage::start()
{
}
