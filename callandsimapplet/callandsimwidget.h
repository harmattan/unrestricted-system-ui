#ifndef CALLANDSIMWIDGET_H
#define CALLANDSIMWIDGET_H

#include "dcpwidget.h"

class CallContainer;
class SimContainer;
class ForwardingContainer;
class CallAndSim;

class CallAndSimWidget : public DcpWidget
{
    Q_OBJECT

public:
    CallAndSimWidget(QGraphicsWidget* parent = 0);

private:
    CallContainer* callContainer;
    SimContainer* simContainer;
    ForwardingContainer* forwardingContainer;
};

#endif // CALLANDSIMWIDGET_H
