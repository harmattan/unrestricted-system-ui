#ifndef CALLCONTAINER_H
#define CALLCONTAINER_H

#include <DuiContainer>
#include <QString>

class CallContainer : public DuiContainer
{
public:
    CallContainer(const QString &title, DuiWidget *parent);
};

#endif // CALLCONTAINER_H
