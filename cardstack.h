#ifndef CARDSTACK_H
#define CARDSTACK_H

#include "card.h"

class CardStack
{
public:
    virtual ~CardStack() = default;
    virtual Card& top() = 0;
    virtual Card pop() = 0;
    virtual void push(Card &card) = 0;
    virtual short rows() = 0;
    virtual std::string getRow(short row) = 0;
    virtual bool isEmpty() = 0;
    virtual short size() = 0;
    virtual Card at(short index) = 0;
};


#endif // CARDSTACK_H
