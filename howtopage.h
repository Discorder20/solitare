#ifndef HOWTOPAGE_H
#define HOWTOPAGE_H

#include "page.h"

class HowtoPage : public Page
{
public:
    HowtoPage();
    ~HowtoPage();

    void render() override;
    void handleKey(char key) override;
    void start() override;
};

#endif // HOWTOPAGE_H
