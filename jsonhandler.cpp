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
    m_json["current_step"] = MAX_STEPS_BACK;
    m_json["game_save"]["current_move"] = 0;
    m_json["game_save"]["moves"] = json::array();
    m_json["game_save"]["extras_stack"] = json::array();
    m_json["game_save"]["build_stack"] = json::array();
    m_json["game_save"]["regular_stack"] = json::array();
    m_json["game_save"]["records"] = json::array();
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
    file << std::setw(2) << m_json;
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
    bool containsIndex = container.size() > index;
    bool isNull = container[index].is_null();
    if (containsIndex && !isNull) {
        cards = container[index].size();
    } else {
        cards = 0;
    }
    for (short i = 0; i < cards; ++i) {
        short value = container[index][i]["value"].get<short>();
        short color = container[index][i]["color"].get<short>();
        bool hidden = container[index][i]["hidden"].get<short>();
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
    return std::to_string(index);
}

void JsonHandler::saveMove(short selectedRow, short selectedColumn, short currentRow, short currentColumn, Card card, bool counted)
{
    if (counted) {
        short value = m_json["game_save"]["current_move"].get<short>();
        m_json["game_save"]["current_move"] = value + 1;
    }

    m_json["game_save"]["moves"].push_back({
                                            {"selectedRow", selectedRow},
                                            {"selectedColumn", selectedColumn},
                                            {"currentRow", currentRow},
                                            {"currentColumn", currentColumn},
                                            {"card", serializeCard(card)},
                                            {"multiple", false}
                                            });

    update();
}


void JsonHandler::moveMultipleCards(short selectedRow, short selectedColumn, short currentRow, short currentColumn, short rows)
{
    short size = m_json["game_save"]["regular_stack"][selectedColumn].size();
    short value = m_json["game_save"]["current_move"].get<short>();
    for (short i = rows - 1; i >= 0; --i) {
        short index = size - (1 + i);
        m_json["game_save"]["moves"].push_back({
                                                {"selectedRow", selectedRow},
                                                {"selectedColumn", selectedColumn},
                                                {"currentRow", currentRow},
                                                {"currentColumn", currentColumn},
                                                {"card", m_json["game_save"]["regular_stack"][selectedColumn].at(index)},
                                                {"multiple", true},
                                                {"rows", rows}

                                                });
    }
    m_json["game_save"]["current_move"] = value + 1;

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

void JsonHandler::clearFile()
{
    m_json["current_step"] = MAX_STEPS_BACK;
    m_json["game_save"]["current_move"] = 0;
    m_json["game_save"]["moves"] = json::array();
    m_json["game_save"]["extras_stack"] = json::array();
    m_json["game_save"]["build_stack"] = json::array();
    m_json["game_save"]["regular_stack"] = json::array();
    m_json["game_save"]["records"] = json::array();
    update();
}

void JsonHandler::serializeCards(CardContainer* extrasStack, CardContainer* buildStack, CardContainer* regularStack)
{
    m_json["game_save"]["extras_stack"] = json::array();
    m_json["game_save"]["build_stack"] = json::array();
    m_json["game_save"]["regular_stack"] = json::array();

    for (short i = 0; i < EXTRAS_STACK_SIZE; ++i) {
        short size = extrasStack->getCol(i)->size();
        m_json["game_save"]["extras_stack"][i] = json::array();
        for (short j = 0; j < size; ++j) {
            json cardJson = serializeCard(extrasStack->getCol(i)->at(j));
            m_json["game_save"]["extras_stack"][i].insert(m_json["game_save"]["extras_stack"][i].end(), cardJson);
        }
    }

    for (short i = 0; i < BUILD_STACK_SIZE; ++i) {
        short size = buildStack->getCol(i)->size();
        m_json["game_save"]["build_stack"][i] = json::array();
        for (short j = 0; j < size; ++j) {
            json cardJson = serializeCard(buildStack->getCol(i)->at(j));
            m_json["game_save"]["build_stack"][i].insert(m_json["game_save"]["build_stack"][i].end(), cardJson);
        }
    }

    for (short i = 0; i < REGULAR_STACK_SIZE; ++i) {
        short size = regularStack->getCol(i)->size();
        m_json["game_save"]["regular_stack"][i] = json::array();
        for (short j = 0; j < size; ++j) {
            json cardJson = serializeCard(regularStack->getCol(i)->at(j));
            m_json["game_save"]["regular_stack"][i].insert(m_json["game_save"]["regular_stack"][i].end(), cardJson);
        }
    }

    update();
}

short JsonHandler::getMoves()
{
    return m_json["game_save"]["current_move"].get<short>();
}

void JsonHandler::addRecord(short moves)
{
    m_json["game_save"]["records"].push_back({{"moves", moves}});
}

short JsonHandler::recordSize()
{
    return m_json["game_save"]["records"].size();
}

Record *JsonHandler::getRecords()
{
    short size = m_json["game_save"]["records"].size();
    Record* records = new Record[size];
    for (short i = 0; i < size; ++i) {
        records[i].moves = m_json["game_save"]["records"][i]["moves"].get<short>();
    }

    bool swapped;

    for (short i = 0; i < size - 1; ++i) {
        swapped = false;
        for (short j = 0; j < size - 1 - i; ++j) {
            // Compare adjacent elements
            if (records[j].moves > records[j+1].moves) {
                // Swap them if they are in the wrong order
                Record temp = records[j];
                records[j] = records[j+1];
                records[j+1] = temp;
                swapped = true;
            }
        }
        // If no two elements were swapped by inner loop, then array is sorted
        if (!swapped) {
            break;
        }
    }

    return records;
}

Move JsonHandler::popLastMove()
{
    short moves = m_json["game_save"]["moves"].size();
    json lastMove = m_json["game_save"]["moves"][moves - 1];
    Move move;
    move.selectedRow = lastMove["selectedRow"].get<short>();
    move.selectedColumn = lastMove["selectedColumn"].get<short>();
    move.currentRow = lastMove["currentRow"].get<short>();
    move.currentColumn = lastMove["currentColumn"].get<short>();
    move.multiple = lastMove["multiple"].get<bool>();

    if (lastMove.contains("rows")) {
        move.rows = lastMove["rows"].get<short>();
    } else {
        move.rows = 1;
    }

    move.card = Card(
        lastMove["card"]["value"].get<short>(),
        lastMove["card"]["color"].get<short>(),
        lastMove["card"]["hidden"].get<short>()
    );

    m_json["game_save"]["moves"].erase(moves - 1);

    return move;
}

short JsonHandler::getAvailableMoves()
{
    short moves = m_json["game_save"]["moves"].size();
    return moves;
}

void JsonHandler::setCurrentStep(short step)
{
    m_json["current_step"] = step;
    update();
}

short JsonHandler::getCurrentStep()
{
    return m_json["current_step"].get<short>();
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
