#include "regularcardstack.h"
#include "constants.h"

using namespace std;

RegularCardStack::RegularCardStack()
{
}

RegularCardStack::RegularCardStack(Card cards[], int size)
{
    for (short i = 0; i < size; ++i) {
        m_cardStack.push(cards[i]);
    }
}

RegularCardStack::~RegularCardStack()
{
}

Card& RegularCardStack::top()
{
    return m_cardStack.top();
}

Card RegularCardStack::pop()
{
    Card topCard(top());
    m_cardStack.pop();
    return topCard;
}

void RegularCardStack::push(Card &card)
{
    m_cardStack.push(card);
}

short RegularCardStack::rows()
{
    return CARD_HEIGHT;
}

std::string RegularCardStack::getRow(short row)
{
    if (isEmpty()) {
        Card tempCard(0, 0, true); // for showing empty card field;
        return tempCard.getRow(row, true);
    } else {
        return m_cardStack.top().getRow(row);
    }
}

bool RegularCardStack::isEmpty()
{
    return m_cardStack.size() == 0;
}

short RegularCardStack::size()
{
    return m_cardStack.size();
}

Card RegularCardStack::at(short index)
{
    std::stack<Card> tempStack = m_cardStack;

    for (short i = 0; i < m_cardStack.size() - 1 - index; ++i) {
        tempStack.pop();
    }
    return tempStack.top();
}
