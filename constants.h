#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const int PAGES_COUNT = 6; // 5 pages + menu
const int COLOR_BLACK = 0;
const int COLOR_BLUE = 1;
const int COLOR_GREEN = 2;
const int COLOR_CYAN = 3; // Also known as Aqua
const int COLOR_RED = 4;
const int COLOR_MAGENTA = 5; // Also known as Purple
const int COLOR_YELLOW = 6;
const int COLOR_WHITE = 7; // Also known as Light Gray
const int COLOR_GRAY = 8; // Also known as Dark Gray
const int COLOR_LIGHT_BLUE = 9;
const int COLOR_LIGHT_GREEN = 10;
const int COLOR_LIGHT_CYAN = 11; // Also known as Light Aqua
const int COLOR_LIGHT_RED = 12;
const int COLOR_LIGHT_MAGENTA = 13; // Also known as Light Purple
const int COLOR_LIGHT_YELLOW = 14;
const int COLOR_BRIGHT_WHITE = 15;
const int ENTER_KEY = 13;
const int ESCAPE_KEY = 27;
const int A_KEY = 97;
const int D_KEY = 100;
const int CARD_HEIGHT = 6;
const int CARD_WIDTH = 8;
const int OUTSTANDING_CARD_HEIGHT = 2;
const int INTERNAL_CONTAINER_SPACING = 1;
const int EXTERNAL_CONTAINER_SPACING = 3;
const int EXTRAS_STACK = 0;
const int BUILD_STACK = 1;
const int REGULAR_STACK = 2;
const int EXTRAS_STACK_SIZE = 2;
const int BUILD_STACK_SIZE = 4;
const int REGULAR_STACK_SIZE = 7;
const int REGULAR_CARD_STACK = 0;
const int OUTSTANDING_CARD_STACK = 1;
const int CARD_DECK_SIZE = 52;
const int CARDS_FOR_REGULAR = 28; // card needed for regular stack;
const int CARDS_FOR_EXTRA = 24;
const std::string UNKNOWN_CARD_SYMBOL = "?";
const std::string JSON_FILE_NAME = "data.json";
const std::string TOP_LEFT_CORNER = "╭";
const std::string TOP_RIGHT_CORNER = "╮";
const std::string BOTTOM_LEFT_CORNER = "╰";
const std::string BOTTOM_RIGHT_CORNER = "╯";
const std::string WALL = "│";
const std::string CORNER = "─";
const std::string CHARACTERS[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const std::string COLORS[] = {"♥", "♠", "♦", "♣"};

#endif // CONSTANTS_H
