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
    , m_selected(false)
    , m_gameWon(false)
    , m_currentStep(handler.getCurrentStep())
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
    m_handler.clearFile();
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
    m_selected = false;
    m_gameWon = false;
    m_currentStep = m_handler.getCurrentStep();
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
            m_handler.addCard(2, i, deck[index]);
            regularStack[i]->push(deck[index++]);
        }
        deck[index].show();
        m_handler.addCard(2, i, deck[index]);
        regularStack[i]->push(deck[index++]);
    }

    m_regularStack = new CardContainer(REGULAR_STACK_SIZE, regularStack);
}

void Game::allocateExtrasStack(Card deck[])
{
    CardStack** extrasStack = new CardStack*[EXTRAS_STACK_SIZE];
    extrasStack[0] = new RegularCardStack();

    for (short i = CARDS_FOR_REGULAR; i < CARD_DECK_SIZE; ++i) {
        m_handler.addCard(0, 0, deck[i]);
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
    m_currentStep = m_handler.getCurrentStep();
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
        if (m_currentRow == 0 && m_currentColumn >= BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) {
            m_currentColumn = (BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) - 1;
        }
    }
}

void Game::moveBottom()
{
    if (m_currentRow < m_regularStack->getCol(m_currentColumn)->size() || (m_regularStack->getCol(m_currentColumn)->size() == 0 && m_currentRow == 0)) {
        m_currentRow++;
    }
}

void Game::moveLeft()
{
    if (m_currentColumn > 0) {
        short leftColumnSize = m_regularStack->getCol(m_currentColumn - 1)->size();
        if (m_currentRow > leftColumnSize) {
            if (leftColumnSize == 0) {
                m_currentRow = 1;
            } else {
                m_currentRow = leftColumnSize;
            }
        }
        m_currentColumn--;
    }
}

void Game::moveRight()
{
    if ((m_currentRow == 0 && m_currentColumn == (BUILD_STACK_SIZE + EXTRAS_STACK_SIZE) - 1) || ( m_currentRow != 0 && m_currentColumn == REGULAR_STACK_SIZE - 1)) {
        return ;
    }
    short rightColumnSize = m_regularStack->getCol(m_currentColumn + 1)->size();
    if (m_currentRow > rightColumnSize) {
        if (rightColumnSize == 0) {
            m_currentRow = 1;
        } else {
            m_currentRow = rightColumnSize;
        }
    }
    m_currentColumn++;
}

void Game::handleClick()
{
    if (m_currentColumn == 0 && m_currentRow == 0) {
        moveExtraCard();
    } else if ((m_currentRow == 0 && m_currentColumn > 0) || m_currentRow > 0) {
        handleCardClick();
    }
}

void Game::moveExtraCard()
{
    m_selected = false; // cancel selecting
    string mode = m_handler.getMode();
    short cards = (mode == "easy") ? 1 : 3;
    if (m_extrasStack->getCol(0)->size() >= cards) {
        for (short i = 0; i < cards; ++i) {
            m_extrasStack->getCol(1)->push(m_extrasStack->getCol(0)->top());
            m_handler.saveMove(0, 0, 0, 1, m_extrasStack->getCol(0)->top(), i == 0);
            m_extrasStack->getCol(0)->pop();
        }
        m_extrasStack->getCol(1)->top().show();

    } else {
        Card cards[CARDS_FOR_EXTRA];
        short size = m_extrasStack->getCol(1)->size();
        for (short i = 0; i < size; ++i) {
            cards[i] = m_extrasStack->getCol(1)->at(i);
        }

        shuffleDeck(cards, size);

        for (short i = 0; i < size; ++i) {
            m_extrasStack->getCol(0)->push(cards[i]);
            m_extrasStack->getCol(0)->top().hide();
            m_extrasStack->getCol(1)->pop();
        }
    }

    m_handler.serializeCards(m_extrasStack, m_buildStack, m_regularStack);

}

void Game::handleCardClick()
{
    bool selectingEmptyRegular = (m_currentRow > 0 && m_regularStack->getCol(m_currentColumn)->size() == 0);
    bool selectable = isSelectionAvailable();
    bool pushingBuildStack = m_currentRow == 0 && m_currentColumn >= EXTRAS_STACK_SIZE && m_selected;
    bool selectingEmptyExtrasStack = m_currentRow == 0 && m_currentColumn == 1 && m_extrasStack->getCol(m_currentColumn)->size() == 0;

    if (selectingEmptyRegular || selectable || pushingBuildStack) {
        if (!m_selected && !selectingEmptyRegular && !pushingBuildStack && !selectingEmptyExtrasStack) {
            m_selected = true;
            m_selectedColumn = m_currentColumn;
            m_selectedRow = m_currentRow;
        } else {
            handleSubmit();
        }
    }
}

void Game::handleSubmit() // it's gonna be a hard one!!
{
    if (m_currentColumn == m_selectedColumn && m_currentRow == m_selectedRow) {
        return ;
    }

    CardStack* selectedStack = getSelectedStack();
    if (m_selectedRow > 0) { // moving from regular stack
        if (m_currentRow == 0) { // moving from regular stack to extras or build stack
            if (m_currentColumn >= EXTRAS_STACK_SIZE ) { // moving to build stack
                short currentValue = selectedStack->at(m_selectedRow - 1).getValue();
                short currentColor = selectedStack->at(m_selectedRow - 1).getColor();
                short buildColumn = m_currentColumn - EXTRAS_STACK_SIZE;
                if (m_buildStack->getCol(buildColumn)->isEmpty()) {
                    if (currentValue == BOTTOM_SYMBOL) {
                        m_buildStack->getCol(buildColumn)->push(m_regularStack->getCol(m_selectedColumn)->top());
                        registerMove(m_regularStack->getCol(m_selectedColumn)->top());
                        m_regularStack->getCol(m_selectedColumn)->pop();
                        if (m_regularStack->getCol(m_selectedColumn)->size() > 0) {
                            m_regularStack->getCol(m_selectedColumn)->top().show();
                        }
                    }
                } else if (m_buildStack->getCol(buildColumn)->top().getValue() == currentValue - 1 && currentColor == m_buildStack->getCol(buildColumn)->top().getColor()) { // <
                    m_buildStack->getCol(buildColumn)->push(m_regularStack->getCol(m_selectedColumn)->top());
                    registerMove(m_regularStack->getCol(m_selectedColumn)->top());
                    m_regularStack->getCol(m_selectedColumn)->pop();
                    if (m_regularStack->getCol(m_selectedColumn)->size() > 0) {
                        m_regularStack->getCol(m_selectedColumn)->top().show();
                    }
                }
            }
        } else { // moving from regular stack to regular stack
            short currentValue = selectedStack->at(m_selectedRow - 1).getValue();
            short currentColor = selectedStack->at(m_selectedRow - 1).getColor();
            if (m_regularStack->getCol(m_currentColumn)->isEmpty()) {
                if (currentValue == TOP_SYMBOL) {
                    Card cards[CARDS_FOR_EXTRA];
                    short selectedCards = m_regularStack->getCol(m_selectedColumn)->size() - m_selectedRow;
                    for (short i = 0; i <= selectedCards; ++i) {
                        cards[i] = m_regularStack->getCol(m_selectedColumn)->top();
                        m_regularStack->getCol(m_selectedColumn)->pop();
                    }

                    m_handler.moveMultipleCards(m_selectedRow, m_selectedColumn, m_currentRow, m_currentColumn, selectedCards + 1);

                    for (short i = selectedCards; i >= 0; --i) {
                        m_regularStack->getCol(m_currentColumn)->push(cards[i]);
                    }

                    if (m_regularStack->getCol(m_selectedColumn)->size() > 0) {
                        m_regularStack->getCol(m_selectedColumn)->top().show();
                    }
                }
            } else if ( m_regularStack->getCol(m_currentColumn)->top().getValue() == currentValue + 1 && currentColor % 2 != m_regularStack->getCol(m_currentColumn)->top().getColor() % 2) { // >
                Card cards[CARDS_FOR_EXTRA];
                short selectedCards = m_regularStack->getCol(m_selectedColumn)->size() - m_selectedRow;
                for (short i = 0; i <= selectedCards; ++i) {
                    cards[i] = m_regularStack->getCol(m_selectedColumn)->top();
                    m_regularStack->getCol(m_selectedColumn)->pop();
                }

                m_handler.moveMultipleCards(2, m_selectedColumn, 2, m_currentColumn, selectedCards + 1);

                for (short i = selectedCards; i >= 0; --i) {
                    m_regularStack->getCol(m_currentColumn)->push(cards[i]);
                }

                if (m_regularStack->getCol(m_selectedColumn)->size() > 0) {
                    m_regularStack->getCol(m_selectedColumn)->top().show();
                }
            }
        }
    } else if (m_selectedColumn == 1) { // moving from extras stack
        if (m_currentRow == 0) { // moving from extras stack to extras or build stack
            if (m_currentColumn >= EXTRAS_STACK_SIZE ) { // moving from extras stack to build stack
                short currentValue = selectedStack->top().getValue();
                short currentColor = selectedStack->top().getColor();
                short buildColumn = m_currentColumn - EXTRAS_STACK_SIZE;
                if (m_buildStack->getCol(buildColumn)->isEmpty()) {
                    if (currentValue == BOTTOM_SYMBOL) {
                        m_buildStack->getCol(buildColumn)->push(m_extrasStack->getCol(m_selectedColumn)->top());
                        registerMove(m_extrasStack->getCol(1)->top());
                        m_extrasStack->getCol(1)->pop();
                    }
                } else if (m_buildStack->getCol(buildColumn)->top().getValue() == currentValue - 1 && currentColor == m_buildStack->getCol(buildColumn)->top().getColor()) { // <
                    m_buildStack->getCol(buildColumn)->push(m_extrasStack->getCol(m_selectedColumn)->top());
                    registerMove(m_extrasStack->getCol(1)->top());
                    m_extrasStack->getCol(1)->pop();
                }
            }
        } else { // moving from extras stack to regular stack
            short currentValue = selectedStack->top().getValue();
            short currentColor = selectedStack->top().getColor();
            if (m_regularStack->getCol(m_currentColumn)->isEmpty()) {
                if (currentValue == TOP_SYMBOL) {
                    m_regularStack->getCol(m_currentColumn)->push(m_extrasStack->getCol(m_selectedColumn)->top());
                    registerMove(m_extrasStack->getCol(1)->top());
                    m_extrasStack->getCol(m_selectedColumn)->pop();
                }
            } else if ( m_regularStack->getCol(m_currentColumn)->top().getValue() == currentValue + 1 && currentColor % 2 != m_regularStack->getCol(m_currentColumn)->top().getColor() % 2) { // >
                m_regularStack->getCol(m_currentColumn)->push(m_extrasStack->getCol(m_selectedColumn)->top());
                registerMove(m_extrasStack->getCol(1)->top());
                m_extrasStack->getCol(m_selectedColumn)->pop();
            }
        }
    } else if (m_selectedColumn > 1) { // moving from build stack
        if (m_currentRow != 0) { // moving from build stack to regular stack
            short currentValue = selectedStack->top().getValue();
            short currentColor = selectedStack->top().getColor();
            short buildStackColumn = m_selectedColumn - EXTRAS_STACK_SIZE;
            if (m_regularStack->getCol(m_currentColumn)->isEmpty()) {
                if (currentValue == TOP_SYMBOL) {
                    m_regularStack->getCol(m_currentColumn)->push(m_buildStack->getCol(buildStackColumn)->top());
                    registerMove(m_buildStack->getCol(buildStackColumn)->top());
                    m_buildStack->getCol(buildStackColumn)->pop();
                }
            } else if ( m_regularStack->getCol(m_currentColumn)->top().getValue() == currentValue + 1  && currentColor % 2 != m_regularStack->getCol(m_currentColumn)->top().getColor() % 2) { // >
                m_regularStack->getCol(m_currentColumn)->push(m_buildStack->getCol(buildStackColumn)->top());
                registerMove(m_buildStack->getCol(buildStackColumn)->top());
                m_buildStack->getCol(buildStackColumn)->pop();
            }
        }
    }

    if (m_currentStep < MAX_STEPS_BACK) {
        m_currentStep++;
        m_handler.setCurrentStep(m_currentStep);
    }

    m_handler.serializeCards(m_extrasStack, m_buildStack, m_regularStack);

    cancelSelection();
    checkForWinning();
}

CardStack* Game::getSelectedStack()
{
    if (m_selectedRow == 0) {
        if (m_selectedColumn < EXTRAS_STACK_SIZE) {
            return m_extrasStack->getCol(m_selectedColumn);
        } else {
            return m_buildStack->getCol(m_selectedColumn - EXTRAS_STACK_SIZE);
        }
    } else {
        return m_regularStack->getCol(m_selectedColumn);
    }
}

bool Game::isSelectionAvailable()
{
    if (m_currentRow == 0) {
        if (m_currentColumn <= EXTRAS_STACK_SIZE - 1) {
            if (m_extrasStack->getCol(m_currentColumn)->size() == 0) {
                return false;
            }
            return !m_extrasStack->getCol(m_currentColumn)->top().isHidden();
        } else {
            if (m_buildStack->getCol(m_currentColumn - EXTRAS_STACK_SIZE)->size() == 0) {
                return false;
            }
            return !m_buildStack->getCol(m_currentColumn - EXTRAS_STACK_SIZE)->top().isHidden();
        }
    } else {
        if (m_regularStack->getCol(m_currentColumn)->isEmpty()) {
            return true;
        }
        return !m_regularStack->getCol(m_currentColumn)->at(m_currentRow - 1).isHidden();
    }
}

void Game::checkForWinning()
{
    bool gameWon = true;
    for (short i = 0; i < REGULAR_STACK_SIZE; ++i) {
        if (m_regularStack->getCol(i)->size() != 0) {
            gameWon = false;
        }
    }
    if (gameWon) {
        m_handler.addRecord(m_handler.getMoves());
        m_gameWon = true;
    }
}

void Game::registerMove(Card card, bool counted)
{
    m_handler.saveMove(m_selectedRow, m_selectedColumn , m_currentRow, m_currentColumn, card, counted);
}

CardStack *Game::getStack(short row, short column)
{
    if (row == 0) {
        if (column < EXTRAS_STACK_SIZE) {
            return m_extrasStack->getCol(column);
        } else {
            return m_buildStack->getCol(column - EXTRAS_STACK_SIZE);
        }
    } else {
        return m_regularStack->getCol(column);
    }
}

void Game::cancelSelection()
{
    m_selected = false;
}

bool Game::isGameWon()
{
    return m_gameWon;
}

void Game::cancelMove()
{
    if (m_handler.getAvailableMoves() > 0 && m_currentStep != 0) {
        Move move = m_handler.popLastMove();
        if (move.multiple && move.rows != 1) {
            short targetSize = getStack(move.selectedRow, move.selectedColumn)->size();
            if (targetSize > 0 && (targetSize == 1  || getStack(move.selectedRow, move.selectedColumn)->at(targetSize - 2).isHidden())) {
                getStack(move.selectedRow, move.selectedColumn)->top().hide();
            }

            Move* moves = new Move[move.rows];
            moves[0] = move;

            for (short i = 1; i < move.rows; ++i) {
                Move currentMove = m_handler.popLastMove();
                moves[i] = currentMove;
            }

            for (short i = 0; i < move.rows; ++i) {
                Move currentMove = moves[move.rows - (i + 1)];
                getStack(currentMove.selectedRow, currentMove.selectedColumn)->push(currentMove.card);
                getStack(currentMove.currentRow, currentMove.currentColumn)->pop();
            }

            delete [] moves;
        } else {
            short targetSize = getStack(move.selectedRow, move.selectedColumn)->size();
            if (targetSize > 0 && (targetSize == 1  || getStack(move.selectedRow, move.selectedColumn)->at(targetSize - 2).isHidden())) {
                getStack(move.selectedRow, move.selectedColumn)->top().hide();
            }
            getStack(move.selectedRow, move.selectedColumn)->push(getStack(move.currentRow, move.currentColumn)->top());
            getStack(move.currentRow, move.currentColumn)->pop();
            if (move.selectedRow == 0 && move.selectedColumn == 0) {
                getStack(move.selectedRow, move.selectedColumn)->top().hide();
            }
        }
        m_currentStep--;
        m_handler.setCurrentStep(m_currentStep);
        m_handler.serializeCards(m_extrasStack, m_buildStack, m_regularStack);
    }
}

short Game::getCurrentStep()
{
    return m_currentStep;
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
        } else if (m_selected && m_selectedRow == 0 && m_selectedColumn == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_YELLOW, cols[i]);
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
        } else if (m_selected && m_selectedRow == 0 && m_selectedColumn - EXTRAS_STACK_SIZE == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_YELLOW, cols[i]);
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
        short regularSelectedRow = m_selectedRow - 1;
        short cardNumber = ((row - CARD_HEIGHT) / OUTSTANDING_CARD_HEIGHT);
        short size = m_regularStack->getCol(i)->size();
        short regularLine = row - CARD_HEIGHT;
        short outstandingRows = (m_regularStack->getCol(i)->size() - 1) * OUTSTANDING_CARD_HEIGHT;

        if (((size == 0 && regularRow == 0) || (regularLine > outstandingRows && regularRow == size - 1 && size != 0) || cardNumber == regularRow) && m_currentColumn == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_BLUE, cols[i]);
        } else if (m_selected && ((size == 0 && regularSelectedRow == 0) || (regularLine > outstandingRows && regularSelectedRow == size - 1 && size != 0) || cardNumber == regularSelectedRow) && m_selectedColumn == i) {
            releaseCache();
            printColoredText(COLOR_LIGHT_YELLOW, cols[i]);
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
