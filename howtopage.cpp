#include "howtopage.h"
#include <iostream>

using namespace std;

HowtoPage::HowtoPage()
{
}

HowtoPage::~HowtoPage()
{
}

void HowtoPage::render()
{
    cout << "Sterowanie (Aby wyjść ze sterowania naciśnij przycisk 'Escape')" << endl;
    cout << "\nAutor: Michał Borzuchowski" << endl;
}

void HowtoPage::handleKey(char key)
{
}

void HowtoPage::start()
{
}
