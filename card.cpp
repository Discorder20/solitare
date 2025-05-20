#include "card.h"
#include "constants.h"

using namespace std;

Card::Card()
{
}

Card::Card(short value, short color, bool hidden)
    : m_value(value)
    , m_color(color)
    , m_hidden(hidden)
{
}

Card::~Card()
{
}

void Card::init(short value, short color, bool hidden)
{
    m_value = value;
    m_color = color;
    m_hidden = hidden;
}

void Card::show()
{
    m_hidden = false;
}

void Card::hide()
{
    m_hidden = true;
}

std::string Card::getRow(short row, bool empty)
{
    // DRY over optimization
    string spaces = "";
    for (short i = 0; i < CARD_WIDTH - 2; ++i) {
        spaces += " ";
    }

    string floors= "";
    for (short i = 0; i < CARD_WIDTH - 2; ++i) {
        floors += CORNER;
    }

    const std::string CHARACTER = CHARACTERS[m_value];

    if (row == 0) {
        string output = TOP_LEFT_CORNER + floors + TOP_RIGHT_CORNER;
        return output;
    } else if (row == CARD_HEIGHT - 1) {
        string output = BOTTOM_LEFT_CORNER + floors + BOTTOM_RIGHT_CORNER;
        return output;
    } else if (row == 1) { // top information row
        if (!m_hidden) {
            short extraSpace = (m_value == 9 ? 3 : 2); // if it's '10', we need to reduce spaces amount
            string output = WALL + CHARACTER + spaces.substr(0, spaces.size() - extraSpace) + COLORS[m_color] + WALL;
            return output;
        } else {
            string output;
            if (!empty) {
                short extraSpace = 2;
                output = WALL + UNKNOWN_CARD_SYMBOL + spaces.substr(0, spaces.size() - extraSpace) + UNKNOWN_CARD_SYMBOL + WALL;
            } else {
                output = WALL + spaces + WALL;
            }
            return output;
        }
    } else if (row == CARD_HEIGHT - 2) { // bottom information row
        if (!m_hidden) {
            short extraSpace = (m_value == 9 ? 3 : 2); // if it's '10', we need to reduce spaces amount
            string output = WALL + COLORS[m_color] + spaces.substr(0, spaces.size() - extraSpace) + CHARACTER + WALL;
            return output;
        } else {
            string output;
            if (!empty) {
                short extraSpace = 2;
                output = WALL + UNKNOWN_CARD_SYMBOL + spaces.substr(0, spaces.size() - extraSpace) + UNKNOWN_CARD_SYMBOL + WALL;
            } else {
                output = WALL + spaces + WALL;
            }
            return output;
        }
    } else {
        string output = WALL + spaces + WALL;
        return output;
    }
}

short Card::getValue()
{
    return m_value;
}

short Card::getColor()
{
    return m_color;
}

short Card::isHidden()
{
    return m_hidden;
}
