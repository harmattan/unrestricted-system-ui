#include "callandsimwidget.h"
#include "dcpcallandsim.h"
#include "callcontainer.h"
#include "simcontainer.h"
#include "callandsimtranslation.h"

CallAndSimWidget::CallAndSimWidget(QGraphicsWidget* parent) :
        DcpWidget(parent),
        callContainer(NULL),
        simContainer(NULL)
{
    setReferer(DcpCallAndSim::None);
    initWidget();
}

CallAndSimWidget::~CallAndSimWidget()
{
}

void CallAndSimWidget::initWidget()
{
    // create dbus

    // connect dbus if signals

    // get initial values

    // create containers
    callContainer = new CallContainer(this);
    simContainer = new SimContainer(this);

    // connect signals, containers -> this

    // main layout
}
