#ifndef CALLANDSIMWIDGET_H
#define CALLANDSIMWIDGET_H

#include "dcpwidget.h"

class CallContainer;
class SimContainer;
class CallAndSimDBusInterface;

class CallAndSimWidget : public DcpWidget
{
    Q_OBJECT

public:
    CallAndSimWidget(QGraphicsWidget* parent = 0);
    virtual ~CallAndSimWidget();

protected:
    void initWidget();

private:
    CallContainer* callContainer;
    SimContainer* simContainer;
    CallAndSimDBusInterface* dbusIf;
};

#endif // CALLANDSIMWIDGET_H
