#include "callandsimwidget.h"
#include "dcpcallandsim.h"
#include "callcontainer.h"
#include "simcontainer.h"
#include "callandsimtranslation.h"
#include "callandsim.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

CallAndSimWidget::CallAndSimWidget(QGraphicsWidget* parent) :
        DcpWidget(parent),
        callContainer(NULL),
        simContainer(NULL)
{
    setReferer(DcpCallAndSim::None);

    // create logic

    logic = new CallAndSim(this);

    // create containers

    simContainer = new SimContainer(this);
    callContainer = new CallContainer(this);
//    forwardingContainer = new ForwardingContainer(this);

    // connect signals, containers -> logic

    connect(simContainer, SIGNAL(pinRequestChanged(bool)),               logic, SLOT(setPinRequest(bool)));
    connect(simContainer, SIGNAL(pinChangeRequested()),                  logic, SLOT(changePinCode()));
    connect(callContainer, SIGNAL(sendCallerIdChanged(int)),             logic, SLOT(setCallerIdSending(int)));
    connect(callContainer, SIGNAL(callWaitingChanged(bool)),             logic, SLOT(setCallWaiting(bool)));
//    connect(callContainer, SIGNAL(callForwardingChanged(bool, QString)), logic, SLOT(setCallForwarding(bool, QString)));

    // connect signals, logic -> containers

    connect(logic, SIGNAL(callerIdSendingComplete(int)),          callContainer, SLOT(setSendCallerId(int)));
    connect(logic, SIGNAL(callWaitingComplete(bool)),             callContainer, SLOT(setCallWaiting(bool)));
//    connect(logic, SIGNAL(callForwardingComplete(bool, QString)), callContainer, SLOT(setCallForwarding(bool, QString)));
    connect(logic, SIGNAL(pinRequestComplete(bool)),              simContainer, SLOT(setPinRequest(bool)));

    connect(logic, SIGNAL(requestFailed(DcpCallAndSim::Data)),    callContainer, SLOT(requestFailed(DcpCallAndSim::Data)));
    connect(logic, SIGNAL(requestFailed(DcpCallAndSim::Data)),    simContainer, SLOT(requestFailed(DcpCallAndSim::Data)));

    // get data

    logic->requestData(DcpCallAndSim::AllData);

    // main layout

    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(simContainer);
    mainLayoutPolicy->addItem(callContainer);
//    mainLayoutPolicy->addItem(forwardingContainer);
    mainLayoutPolicy->setSpacing(10);

    this->setLayout(mainLayout);
}
