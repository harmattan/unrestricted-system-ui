#include "callandsimwidget.h"
#include "dcpcallandsim.h"
#include "callcontainer.h"
#include "simcontainer.h"
#include "callandsimtranslation.h"
#include "callandsimdbusinterface.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

CallAndSimWidget::CallAndSimWidget(QGraphicsWidget* parent) :
        DcpWidget(parent),
        callContainer(NULL),
        simContainer(NULL),
        dbusIf(NULL)
{
    setReferer(DcpCallAndSim::None);
    initWidget();
}

CallAndSimWidget::~CallAndSimWidget()
{
    delete dbusIf;
    dbusIf = NULL;
}

void CallAndSimWidget::initWidget()
{
    // create containers

    callContainer = new CallContainer(this);
    simContainer = new SimContainer(this);

    // create dbus

    dbusIf = new CallAndSimDBusInterface();

    // connect signals, containers -> dbusIf

    connect(callContainer, SIGNAL(sendCallerIdChanged(int)),    dbusIf, SLOT(setCallerIdSending(int)));
    connect(callContainer, SIGNAL(callWaitingChanged(bool)),    dbusIf, SLOT(setCallWaiting(bool)));
    connect(callContainer, SIGNAL(callForwardingChanged(bool)), dbusIf, SLOT(setCallForwarding(bool)));
    connect(callContainer, SIGNAL(forwardToChanged(QString)),   dbusIf, SLOT(setForwardTo(QString)));
    connect(simContainer,  SIGNAL(valueChanged(bool)),          dbusIf, SLOT(setPinRequest(bool)));

    // connect signals, dbusIf -> containers

    connect(dbusIf, SIGNAL(callerIdSending(int)), callContainer, SLOT(setSendCallerId(int)));
    connect(dbusIf, SIGNAL(callWaiting(bool)),    callContainer, SLOT(setCallWaiting(bool)));
    connect(dbusIf, SIGNAL(callForwarding(bool)), callContainer, SLOT(setCallForwarding(bool)));
    connect(dbusIf, SIGNAL(forwardTo(QString)),   callContainer, SLOT(setForwardTo(QString)));
    connect(dbusIf, SIGNAL(pinRequest(bool)),     simContainer, SLOT(setPinRequest(bool)));

    dbusIf->requestAllValues();

    // main layout

    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(callContainer);
    mainLayoutPolicy->addItem(simContainer);
    mainLayoutPolicy->setSpacing(10);

    this->setLayout(mainLayout);
}
