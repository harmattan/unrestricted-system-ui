#ifndef NETWORK_H
#define NETWORK_H

#include <DuiWidget>

class DuiStatusIndicatorMenuInterface;

class Network : public DuiWidget
{
    Q_OBJECT

public:
    Network(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent = NULL);
    virtual ~Network();

private:
    //! Interface for controlling the status indicator menu
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu;

};

#endif // NETWORK_H
