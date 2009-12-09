#include "callandsimwidget.h"
#include "dcpcallandsim.h"
#include "callcontainer.h"
#include "simcontainer.h"
#include "forwardingcontainer.h"
#include "callandsimtranslation.h"
#include "callandsim.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

CallAndSimWidget::CallAndSimWidget(QGraphicsWidget* parent) :
        DcpWidget(parent),
        callContainer(NULL),
        simContainer(NULL)
{
    qDebug() << Q_FUNC_INFO;
    setReferer(DcpCallAndSim::None);

    // create containers

    simContainer = new SimContainer;
    callContainer = new CallContainer;
    forwardingContainer = new ForwardingContainer;

    // main layout

    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(simContainer);
    mainLayoutPolicy->addItem(callContainer);
    mainLayoutPolicy->addItem(forwardingContainer);
    mainLayoutPolicy->setSpacing(10);

    this->setLayout(mainLayout);
}
