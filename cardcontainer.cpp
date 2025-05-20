#include "cardcontainer.h"
#include "constants.h"

using namespace std;

CardContainer::CardContainer(short cols, CardStack *cardStacks[])
    : cardStacks(cardStacks)
    , cols(cols)
{
}

CardContainer::~CardContainer()
{
    for(short i = 0; i < cols; i++)
    {
        delete cardStacks[i];
    }
    delete [] cardStacks;
}

short CardContainer::rows()
{
    short maxHeight = 0;

    for(short i = 0; i < cols; i++)
    {
        short rows = cardStacks[i]->rows();
        if (rows > maxHeight) {
            maxHeight = rows;
        }
    }

    return maxHeight;
}

std::string* CardContainer::getRow(short row)
{
    string* outputArray = new string[cols]; // it will be deleted in place where it's used;
    for(short i = 0; i < cols; i++)
    {
        string output = "";
        short rows = cardStacks[i]->rows();
        if (row < rows) {
            output = cardStacks[i]->getRow(row);
        } else {
            output.append(CARD_WIDTH, ' ');
        }
        output.append(INTERNAL_CONTAINER_SPACING, ' ');
        outputArray[i] = output;
    }

    return outputArray;
}

CardStack *CardContainer::getCol(short col)
{
    return cardStacks[col];
}
