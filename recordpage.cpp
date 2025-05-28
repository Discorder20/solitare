#include "recordpage.h"
#include "colors.h"
#include "constants.h"

#include <iostream>

using namespace std;

RecordPage::RecordPage(JsonHandler &handler)
    : m_handler(handler)
{
}

RecordPage::~RecordPage()
{
}

void RecordPage::render()
{
    short size = m_handler.recordSize();
    Record* records = m_handler.getRecords();

    cout << "Tablica Wyników: \n" << endl;

    for (short i = 0; i < size; ++i) {
        cout << to_string(records[i].moves) << endl;
    }

    delete [] records;

}

void RecordPage::handleKey(char key)
{
}

void RecordPage::start()
{
}
