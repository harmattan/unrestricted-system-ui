#include "callandsimwidget.h"
#include "dcpcallandsim.h"

CallAndSimWidget::CallAndSimWidget(QGraphicsWidget* parent) :
        DcpWidget(parent)
{
    setReferer(DcpCallAndSim::None);
    initWidget();
}

CallAndSimWidget::~CallAndSimWidget()
{
}

void CallAndSimWidget::initWidget()
{
}
