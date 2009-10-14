#include "callandsimwidget.h"
#include "dcpcallandsim.h"
#include "callcontainer.h"
#include "simcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

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

    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(callContainer);
    mainLayoutPolicy->addItem(simContainer);
    mainLayoutPolicy->setSpacing(10);

    this->setLayout(mainLayout);
}
