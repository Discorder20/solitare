#ifndef CARDCONTAINER_H
#define CARDCONTAINER_H

#include "cardstack.h"

class CardContainer
{
public:
    CardContainer(short cols, CardStack* cardStacks[]);
    ~CardContainer();

    short rows();
    std::string* getRow(short row); // we can't return one big string because of coloring
    CardStack* getCol(short col);

private:
    CardStack** cardStacks;
    short cols;
};

#endif // CARDCONTAINER_H
