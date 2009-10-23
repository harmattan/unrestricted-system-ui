#ifndef CALLANDSIMWIDGET_H
#define CALLANDSIMWIDGET_H

#include "dcpwidget.h"

class CallContainer;
class SimContainer;
class CallAndSim;

class CallAndSimWidget : public DcpWidget
{
    Q_OBJECT

public:
    CallAndSimWidget(QGraphicsWidget* parent = 0);

private:
    CallContainer* callContainer;
    SimContainer* simContainer;
    CallAndSim* logic;
};

#endif // CALLANDSIMWIDGET_H
