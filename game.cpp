#include "game.h"
#include "colors.h"
#include "constants.h"
#include "outstandingcardstack.h"
#include "regularcardstack.h"

#include <iostream>
#include <ctime>

using namespace std;

Game::Game(JsonHandler &handler)
    : m_handler(handler)
    , m_buildStack (nullptr)
    , m_extrasStack (nullptr)
    , m_regularStack (nullptr)
    , m_currentRow(0)
    , m_currentColumn(0)
    , m_selectedRow(0)
    , m_selectedColumn(0)
{
}

Game::~Game()
{
    delete m_buildStack;
    delete m_extrasStack;
    delete m_regularStack;
}

void Game::initializeGame()
{
    Card deck[CARD_DECK_SIZE];
    short index = 0;
    for (short color = 0; color < 4; ++color) { // 4 colors
        for (short value = 0; value < 13; ++value) { // 13 characters
            deck[index++].init(value, color, true);
        }
    }

    shuffleDeck(deck);
    allocateRegularStack(deck);
    allocateExtrasStack(deck);
    allocateBuildStack(deck);
}

void Game::shuffleDeck(Card deck[], short size)
{
    srand(static_cast<unsigned int>(time(NULL)));

    for (int i = size - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);

        Card temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}

void Game::allocateRegularStack(Card deck[])
{
    CardStack** regularStack = new CardStack*[REGULAR_STACK_SIZE];

    short index = 0;
    for (short i = 0; i < REGULAR_STACK_SIZE; ++i) {
        regularStack[i] = new OutstandingCardStack();
        for (short j = 0; j < i; ++j) {
            regularStack[i]->push(deck[index++]);
        }
        deck[index].show();
        regularStack[i]->push(deck[index++]);
    }

    m_regularStack = new CardContainer(REGULAR_STACK_SIZE, regularStack);
}

void Game::allocateExtrasStack(Card deck[])
{
    CardStack** extrasStack = new CardStack*[EXTRAS_STACK_SIZE];
    extrasStack[0] = new RegularCardStack();

    for (short i = CARDS_FOR_REGULAR; i < CARD_DECK_SIZE; ++i) {
        extrasStack[0]->push(deck[i]);
    }

    extrasStack[1] = new RegularCardStack();

    m_extrasStack = new CardContainer(EXTRAS_STACK_SIZE, extrasStack);
}

void Game::allocateBuildStack(Card deck[])
{
    CardStack** buildStack = new CardStack*[BUILD_STACK_SIZE];
    for (short i = 0; i < BUILD_STACK_SIZE; ++i) {
        buildStack[i] = new RegularCardStack();
    }
    m_buildStack = new CardContainer(BUILD_STACK_SIZE, buildStack);
}

void Game::loadGame()
{
    m_buildStack = new CardContainer(BUILD_STACK_SIZE, m_handler.getStacks(BUILD_STACK));
    m_extrasStack = new CardContainer(EXTRAS_STACK_SIZE, m_handler.getStacks(EXTRAS_STACK));
    m_regularStack = new CardContainer(REGULAR_STACK_SIZE, m_handler.getStacks(REGULAR_STACK));
}

short Game::rows()
{
    short topRows = m_extrasStack->rows();
    short bottomRows = m_regularStack->rows();
    return topRows + bottomRows;
}

void Game::printRow(short row)
{
    short topRows = m_extrasStack->rows();
    if (row >= topRows) {
        printRegularStack(row);
    } else {
        printExtrasStack(row);
        printBuildStack(row);
    }
}

void Game::moveTop()
{
    if (m_currentRow > 0) {
        m_currentRow--;
        if (m_currentColumn >= BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) {
            m_currentColumn = (BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) - 1;
        }
    }
}

void Game::moveBottom()
{
    if (m_currentRow != m_regularStack->getCol(m_currentColumn)->size()) {
        m_currentRow++;
    }
}

void Game::moveLeft()
{
    if (m_currentColumn > 0) {
        if (m_currentRow == m_regularStack->getCol(m_currentColumn)->size()) {
            m_currentRow--;
        }
        m_currentColumn--;
    }
}

void Game::moveRight()
{
    if ((m_currentRow == 0 && m_currentColumn == (BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) - 1) || ( m_currentRow != 0 && m_currentColumn == REGULAR_STACK_SIZE - 1)) {
        return ;
    }
    m_currentColumn++;
}

void Game::handleClick()
{
    if (m_currentColumn == 0 && m_currentRow == 0) {
        moveExtraCard();
    }
}

void Game::moveExtraCard()
{
    string mode = m_handler.getMode();
    short cards = (mode == "easy") ? 1 : 3;
    if (m_extrasStack->getCol(0)->size() >= cards) {
        for (short i = 0; i < cards; ++i) {
            m_extrasStack->getCol(1)->push(m_extrasStack->getCol(0)->top());
            m_extrasStack->getCol(0)->pop();
        }
        m_extrasStack->getCol(1)->top().show();
    } else {
        short size = m_extrasStack->getCol(1)->size();
        for (short i = 0; i < size; ++i) {
            m_extrasStack->getCol(1)->top().hide();
            m_extrasStack->getCol(0)->push(m_extrasStack->getCol(1)->top());
            m_extrasStack->getCol(1)->pop();
        }

        Card cards[CARDS_FOR_EXTRA];
        for (short i = 0; i < CARDS_FOR_EXTRA; ++i) {
            cards[i] = m_extrasStack->getCol(0)->at(i);
        }
        shuffleDeck(cards, CARDS_FOR_EXTRA);
        for (short i = 0; i < CARDS_FOR_EXTRA; ++i) {
            m_extrasStack->getCol(0)->pop();
        }
        for (short i = 0; i < CARDS_FOR_EXTRA; ++i) {
            m_extrasStack->getCol(0)->push(cards[i]);
        }
    }
}

void Game::cacheString(std::string text)
{
    m_cachedOutput += text;
}

void Game::releaseCache()
{
    cout << m_cachedOutput;
    m_cachedOutput = "";
}

void Game::printExtrasStack(short row)
{
    string* cols = m_extrasStack->getRow(row);
    for (short i = 0; i < EXTRAS_STACK_SIZE; ++i) {
        if (m_currentRow == 0 && m_currentColumn == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_BLUE, cols[i]);
        } else {
            if (m_extrasStack->getCol(i)->size() != 0) {
                Card currentCard = m_extrasStack->getCol(i)->top();
                if (currentCard.getColor() % 2 == 0 && !currentCard.isHidden()) {
                    releaseCache();
                    printColoredText(COLOR_LIGHT_RED, cols[i]);
                } else {
                    cacheString(cols[i]);
                }
            } else {
                cacheString(cols[i]);
            }
        }
    }
    cacheString("\t");
    delete [] cols;
}

void Game::printBuildStack(short row)
{
    string* cols = m_buildStack->getRow(row);
    for (short i = 0; i < BUILD_STACK_SIZE; ++i) {
        if (m_currentRow == 0 && m_currentColumn - EXTRAS_STACK_SIZE == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_BLUE, cols[i]);
        } else {
            if (m_buildStack->getCol(i)->size() != 0) {
                Card currentCard = m_buildStack->getCol(i)->top();
                if (currentCard.getColor() % 2 == 0 && !currentCard.isHidden()) {
                    releaseCache();
                    printColoredText(COLOR_LIGHT_RED, cols[i]);
                } else {
                    cacheString(cols[i]);
                }
            } else {
                cacheString(cols[i]);
            }
        }
    }
    cacheString("\n");
    releaseCache();
    delete [] cols;
}

void Game::printRegularStack(short row)
{
    short topRows = m_extrasStack->rows();
    string* cols = m_regularStack->getRow(row - topRows);
    for (short i = 0; i < REGULAR_STACK_SIZE; ++i) {
        short regularRow = m_currentRow - 1;
        short cardNumber = ((row - CARD_HEIGHT) / OUTSTANDING_CARD_HEIGHT);
        short size = m_regularStack->getCol(i)->size();
        if ((regularRow == cardNumber || (regularRow == (size - 1) && cardNumber >= size))  && m_currentColumn == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_BLUE, cols[i]);
        } else {
            if (m_regularStack->getCol(i)->size() != 0) {
                Card currentCard;
                if (cardNumber >= size) {
                    currentCard = m_regularStack->getCol(i)->top();
                } else {
                    currentCard = m_regularStack->getCol(i)->at(cardNumber);
                }
                if (currentCard.getColor() % 2 == 0 && !currentCard.isHidden()) {
                    releaseCache();
                    printColoredText(COLOR_LIGHT_RED, cols[i]);
                } else {
                    cacheString(cols[i]);
                }
            } else {
                cacheString(cols[i]);
            }
        }
    }
    cacheString("\n");
    releaseCache();
    delete [] cols;
}
