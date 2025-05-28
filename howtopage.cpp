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
    cout << "Po całym interfejsie można poruszać się za pomocą przycisków W-S-A-D, program nie rozróżnia małych i dużych liter" << endl;
    cout << "Aby zatwierdzić wybór w menu lub wybór karty należy kliknąć Enter, aby opuścić dany ekran należy wcisnąć przycisk Esc" << endl;
    cout << "Kolory kart mają następujące znaczenie: czerwona/biała oznacza zwykłą, niebieska oznacza aktualne położenie kursora a żółta oznacza zaznaczoną" << endl;
    cout << "Aby anulować zaznaczenie karty należy wcisnąć przycisk C" << endl;
    cout << "Aby dobrać kartę ze stosu lub wtasować ponownie karty do stosu wybierz stos w lewym górnym roku i kliknij Enter" << endl;
    cout << "Aby cofnąć ostatni ruch należy wcisnąć 'z'" << endl;
    cout << "Good luck :)" << endl;
    cout << "\nAutor: Michał Borzuchowski" << endl;
}

void HowtoPage::handleKey(char key)
{
}

void HowtoPage::start()
{
}
