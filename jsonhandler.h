#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "card.h"
#include "cardstack.h"

#include <nlohmann/json.hpp>

class JsonHandler
{
public:
    JsonHandler();
    ~JsonHandler();

    void saveMove(short from, short fromRow, short to, short toRow, Card card);
    void moveCard(short from, short fromRow, short to, short toRow);
    void addCard(short container, short row, Card card);
    void changeMode(std::string mode);
    CardStack** getStacks(short container); // will be deleted in place where used
    void clearJson();
    std::string getMode();

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
