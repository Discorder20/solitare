#ifndef MENUPAGE_H
#define MENUPAGE_H

#include "page.h"

class MenuPage : public Page
{
public:
    MenuPage();
    ~MenuPage();

    void render() override;
    void handleKey(char key) override;
    void start() override;

private:
    short m_index;
};

#endif // MENUPAGE_H
