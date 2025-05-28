#include "cardcontainer.h"
#include "constants.h"

using namespace std;

CardContainer::CardContainer(short m_cols, CardStack *m_cardStacks[])
    : m_cardStacks(m_cardStacks)
    , m_cols(m_cols)
{
}

CardContainer::~CardContainer()
{
    for(short i = 0; i < m_cols; i++)
    {
        delete m_cardStacks[i];
    }
    delete [] m_cardStacks;
}

short CardContainer::rows()
{
    short maxHeight = 0;

    for(short i = 0; i < m_cols; i++)
    {
        short rows = m_cardStacks[i]->rows();
        if (rows > maxHeight) {
            maxHeight = rows;
        }
    }

    return maxHeight;
}

std::string* CardContainer::getRow(short row)
{
    string* outputArray = new string[m_cols]; // it will be deleted in place where it's used;
    for(short i = 0; i < m_cols; i++)
    {
        string output = "";
        short rows = m_cardStacks[i]->rows();
        if (row < rows) {
            output = m_cardStacks[i]->getRow(row);
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
    return m_cardStacks[col];
}
