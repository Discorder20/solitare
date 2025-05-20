#include "jsonhandler.h"
#include "constants.h"
#include "outstandingcardstack.h"
#include "regularcardstack.h"

#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace nlohmann;

JsonHandler::JsonHandler()
    : m_json(json::parse(R"({})")) // temp empty json
{
    if (!fileExists()) {
        initFile();
    } else {
        loadJson();
    }
}

JsonHandler::~JsonHandler()
{
}

bool JsonHandler::fileExists()
{
    return std::filesystem::exists(JSON_FILE_NAME);
}

void JsonHandler::initFile()
{
    m_json["mode"] = "easy";
    m_json["game_save"]["current_move"] = 0;
    m_json["game_save"]["moves"] = json::array();
    m_json["game_save"]["extras_stack"] = json::array();
    m_json["game_save"]["build_stack"] = json::array();
    m_json["game_save"]["regular_stack"] = json::array();
    update();
}

void JsonHandler::loadJson()
{
    ifstream file(JSON_FILE_NAME);
    m_json = json::parse(file);
    file.close();
}

void JsonHandler::update()
{
    std::ofstream file(JSON_FILE_NAME, ios::trunc);
    file << m_json;
    file.close();
}

short JsonHandler::sizeOfContainer(short index)
{
    if (index == EXTRAS_STACK) {
        return EXTRAS_STACK_SIZE;
    } else if (index == BUILD_STACK) {
        return BUILD_STACK_SIZE;
    } else if (index == REGULAR_STACK) {
        return REGULAR_STACK_SIZE;
    }
    return 0;
}

short JsonHandler::typeOfStacks(short index)
{
    if (index == EXTRAS_STACK || index == BUILD_STACK) {
        return REGULAR_CARD_STACK;
    } else if (index == REGULAR_STACK) {
        return OUTSTANDING_CARD_STACK;
    }
    return REGULAR_CARD_STACK;
}

void JsonHandler::allocateStack(CardStack *stack, short index, string containerKey)
{
    auto container = m_json["game_save"][containerKey];
    short cards;
    if (container.contains(to_string(index)) && !container[index].is_null()) {
        cards = container[index].size();
    } else {
        cards = 0;
    }
    for (short i = 0; i < cards; ++i) {
        short value = container[index][cards]["value"].get<short>();
        short color = container[index][cards]["color"].get<short>();
        bool hidden = container[index][cards]["hidden"].get<bool>();
        Card card(value, color, hidden);
        stack->push(card);
    }
}

string JsonHandler::keyOfContainer(short index)
{
    if (index == EXTRAS_STACK) {
        return "extras_stack";
    } else if (index == BUILD_STACK) {
        return "build_stack";
    } else if (index == REGULAR_STACK) {
        return "regular_stack";
    }
    return "";
}

void JsonHandler::saveMove(short from, short fromRow, short to, short toRow, Card card)
{
    short value = m_json["game_save"]["current_move"].get<short>();
    m_json["game_save"]["current_move"] = value + 1;
    m_json["game_save"]["moves"].push_back({
                                            {"from", from},
                                            {"fromRow", fromRow},
                                            {"to", to},
                                            {"toRow", toRow},
                                            {"card", serializeCard(card)},
                                            });

    update();
}

void JsonHandler::moveCard(short from, short fromRow, short to, short toRow)
{
    string fromStack = keyOfContainer(from);
    string toStack = keyOfContainer(to);
    short size = m_json["game_save"][fromStack][fromRow].size();
    m_json["game_save"][toStack][toRow].push_back(m_json["game_save"][fromStack][fromRow].at(size - 1));
    m_json["game_save"][fromStack][fromRow].erase(size - 1);
    update();

}

void JsonHandler::addCard(short container, short row, Card card)
{
    string containerKey = keyOfContainer(container);
    json cardJson = serializeCard(card);

    if (!m_json["game_save"][containerKey][row].is_array()) {
        m_json["game_save"][containerKey][row] = json::array();
    }

    m_json["game_save"][containerKey][row];
    m_json["game_save"][containerKey][row].insert(m_json["game_save"][containerKey][row].end(), cardJson);
    update();
}

void JsonHandler::changeMode(std::string mode)
{
    m_json["mode"] = mode;
    update();
}

CardStack **JsonHandler::getStacks(short container)
{
    string containerKey = keyOfContainer(container);
    short size = sizeOfContainer(container);
    short stacksType = typeOfStacks(container);
    CardStack** stacks = new CardStack*[size];
    for (short i = 0; i < size; ++i) {
        if (stacksType == OUTSTANDING_CARD_STACK) {
            stacks[i] = new OutstandingCardStack();
        } else {
            stacks[i] = new RegularCardStack();
        }
        allocateStack(stacks[i], i, containerKey);
    }
    return stacks;
}

void JsonHandler::clearJson()
{
    m_json = json::parse(R"({})");
    initFile();
}

string JsonHandler::getMode()
{
    string mode = m_json["mode"].get<string>();
    return mode;
}

json JsonHandler::serializeCard(Card card)
{
    json j;
    j["value"] = card.getValue();
    j["color"] = card.getColor();
    j["hidden"] = card.isHidden();
    return j;
}
