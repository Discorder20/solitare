#include "outstandingcardstack.h"
#include "constants.h"

using namespace std;

OutstandingCardStack::OutstandingCardStack()
{
}

OutstandingCardStack::OutstandingCardStack(Card cards[], int size)
{
    for (short i = 0; i < size; ++i) {
        m_cardStack.push(cards[i]);
    }
}

OutstandingCardStack::~OutstandingCardStack()
{
}

Card& OutstandingCardStack::top()
{
    return m_cardStack.top();
}

Card OutstandingCardStack::pop()
{
    Card topCard(top());
    m_cardStack.pop();
    return topCard;
}

void OutstandingCardStack::push(Card &card)
{
    m_cardStack.push(card);
}

short OutstandingCardStack::rows()
{
    if (isEmpty()) {
        return CARD_HEIGHT;
    }
    return ((m_cardStack.size() - 1) * OUTSTANDING_CARD_HEIGHT) + CARD_HEIGHT;
}

std::string OutstandingCardStack::getRow(short row)
{
    if (isEmpty()) {
        Card tempCard(0, 0, true); // for showing empty card field;
        return tempCard.getRow(row, true);
    } else {
        short backgroundCardRows = (m_cardStack.size() - 1) * OUTSTANDING_CARD_HEIGHT;
        if (row >= backgroundCardRows) {
            return m_cardStack.top().getRow(row - backgroundCardRows);
        } else {
            short cardRow = row % OUTSTANDING_CARD_HEIGHT; // which row of hidden card is it;
            short cardIndex = row / OUTSTANDING_CARD_HEIGHT;
            std::stack<Card> tempStack = m_cardStack;

            for (short i = 0; i < m_cardStack.size() - 1 - cardIndex; ++i) {
                tempStack.pop();
            }
            return tempStack.top().getRow(cardRow);
        }
    }
}

bool OutstandingCardStack::isEmpty()
{
    return m_cardStack.size() == 0;
}

short OutstandingCardStack::size()
{
    return m_cardStack.size();
}

Card OutstandingCardStack::at(short index)
{
    std::stack<Card> tempStack = m_cardStack;

    for (short i = 0; i < m_cardStack.size() - 1 - index; ++i) {
        tempStack.pop();
    }
    return tempStack.top();
}


