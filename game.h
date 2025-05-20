#ifndef GAME_H
#define GAME_H

#include "cardcontainer.h"
#include "jsonhandler.h"
#include "constants.h"

class Game
{
public:
    Game(JsonHandler& handler);
    ~Game();

    void initializeGame();
    void shuffleDeck(Card deck[], short size = CARD_DECK_SIZE);
    void allocateRegularStack(Card deck[]);
    void allocateExtrasStack(Card deck[]);
    void allocateBuildStack(Card deck[]);
    void loadGame();
    short rows();
    void printRow(short row);
    void moveTop();
    void moveBottom();
    void moveLeft();
    void moveRight();
    void handleClick();
    void moveExtraCard();
    void cancelSelection();

private:
    void cacheString(std::string text);
    void releaseCache();
    void printExtrasStack(short row);
    void printBuildStack(short row);
    void printRegularStack(short row);
    void handleCardClick();
    bool isSelectionAvailable();

private:
    CardContainer* m_extrasStack;
    CardContainer* m_buildStack;
    CardContainer* m_regularStack;

    JsonHandler &m_handler;
    int m_currentRow;
    int m_currentColumn;
    int m_selectedRow;
    int m_selectedColumn;
    bool m_selected;
    std::string m_cachedOutput;
};

#endif // GAME_H
