#ifndef RECORDPAGE_H
#define RECORDPAGE_H

#include "page.h"
#include "jsonhandler.h"

class RecordPage : public Page
{
public:
    RecordPage(JsonHandler &handler);
    ~RecordPage();

    void render() override;
    void handleKey(char key) override;
    void start() override;

private:
    JsonHandler &m_handler;
};

#endif // RECORDPAGE_H
