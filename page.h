#ifndef PAGE_H
#define PAGE_H

class Page
{
public:
    virtual ~Page() = default;
    virtual void start() = 0;
    virtual void render() = 0;
    virtual void handleKey(char key) = 0;
};

#endif // PAGE_H
