#ifndef CARD_H
#define CARD_H

#include <string>

// values: 0-A, 1-9=2-10 then 11=J, 12=Q, 13-K
// colors: 0-♥ 1-♦ 2-♠ 3-♣

class Card
{
public:
    Card();
    Card(short value, short color, bool hidden);
    ~Card();

    void init(short value, short color, bool hidden);
    void show();
    void hide();
    std::string getRow(short row, bool empty = false);
    short getValue();
    short getColor();
    short isHidden();

private:
    short m_value;
    short m_color;
    bool m_hidden;
};

#endif // CARD_H
