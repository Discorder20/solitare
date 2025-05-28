#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "card.h"
#include "cardstack.h"
#include "cardcontainer.h"

#include <nlohmann/json.hpp>

struct Record {
    short moves;
};

struct Move {
    short selectedRow;
    short selectedColumn;
    short currentRow;
    short currentColumn;
    Card card;
    bool multiple;
    short rows;
};

class JsonHandler
{
public:
    JsonHandler();
    ~JsonHandler();

    void saveMove(short selectedRow, short selectedColumn, short currentRow, short currentColumn, Card card, bool counted = true);
    void moveMultipleCards(short selectedRow, short selectedColumn, short currentRow, short currentColumn, short rows);
    void addCard(short container, short row, Card card);
    void changeMode(std::string mode);
    CardStack** getStacks(short container); // pointer will be deleted in place where used
    void clearJson();
    std::string getMode();
    void clearFile();
    void serializeCards(CardContainer *extrasStack, CardContainer *buildStack, CardContainer *regularStack);
    short getMoves();
    void addRecord(short moves);
    short recordSize();
    Record* getRecords();
    Move popLastMove();
    short getAvailableMoves();
    void setCurrentStep(short step);
    short getCurrentStep();

private:
    bool fileExists();
    void initFile();
    void loadJson();
    void update();
    nlohmann::json serializeCard(Card card);
    std::string keyOfContainer(short index);
    short sizeOfContainer(short index);
    short typeOfStacks(short index);
    void allocateStack(CardStack* stack, short index, std::string containerKey);

private:
    nlohmann::ordered_json m_json;
};

#endif // JSONHANDLER_H
