#ifndef REGULARCARDSTACK_H
#define REGULARCARDSTACK_H

#include "cardstack.h"
#include <stack>

class RegularCardStack : public CardStack
{
public:
    RegularCardStack();
    RegularCardStack(Card cards[], int size);
    ~RegularCardStack();

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

#endif // REGULARCARDSTACK_H
