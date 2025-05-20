#ifndef OUTSTANDINGCARDSTACK_H
#define OUTSTANDINGCARDSTACK_H

#include "cardstack.h"
#include <stack>

class OutstandingCardStack : public CardStack
{
public:
    OutstandingCardStack();
    OutstandingCardStack(Card cards[], int size);
    ~OutstandingCardStack();

    Card& top() override;
    Card pop() override;
    void push(Card &card) override;
    short rows() override;
    std::string getRow(short row) override;
    bool isEmpty() override;
    short size() override;
    Card at(short index) override;

private:
    std::stack<Card> m_cardStack;
};

#endif // OUTSTANDINGCARDSTACK_H
