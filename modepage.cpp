#include "modepage.h"
#include "colors.h"
#include "constants.h"

#include <iostream>

using namespace std;

ModePage::ModePage(JsonHandler &handler)
    : m_handler(handler)
{
}

ModePage::~ModePage()
{
}

void ModePage::render()
{
    cout << "Ustawienia (Aby wyjść ze sterowania naciśnij przycisk 'Escape')\n" << endl;
    cout << "Tryb gry:" << endl;
    cout << "<- ";
    string mode = m_handler.getMode();
    if (mode == "easy") {
        printColoredText(COLOR_LIGHT_GREEN, "łatwy");
    } else {
        printColoredText(COLOR_LIGHT_RED, "trudny");
    }
    cout << " ->" << endl;

}

void ModePage::handleKey(char key)
{
    string mode = m_handler.getMode();
    if (key == A_KEY || key == D_KEY) {
        mode = mode == "easy" ? "hard" : "easy";
        m_handler.changeMode(mode);
    }
}

void ModePage::start()
{
}
