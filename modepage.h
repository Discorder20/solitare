#ifndef MODEPAGE_H
#define MODEPAGE_H

#include "jsonhandler.h"
#include "page.h"

class ModePage : public Page
{
public:
    ModePage(JsonHandler &handler);
    ~ModePage();

    void render() override;
    void handleKey(char key) override;
    void start() override;


private:
    JsonHandler &m_handler;
};

#endif // MODEPAGE_H
