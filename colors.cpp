#include "windows.h"
#include "colors.h"
#include "constants.h"

#include <string>
#include <iostream>

using namespace std;

void setConsoleColor(short color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void printColoredLine(short color, string text) {
    setConsoleColor(color);
    std::cout << text << std::endl;
    setConsoleColor(COLOR_WHITE);
}

void printColoredText(short color, string text) {
    setConsoleColor(color);
    std::cout << text;
    setConsoleColor(COLOR_WHITE);
}
