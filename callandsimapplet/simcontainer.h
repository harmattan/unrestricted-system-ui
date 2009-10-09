#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include <DuiContainer>
#include <QString>

class SimContainer : public DuiContainer
{
public:
    SimContainer(const QString &title, DuiWidget *parent);
};

#endif // SIMCONTAINER_H
