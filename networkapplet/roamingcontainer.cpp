#include "roamingcontainer.h"
#include "networktranslation.h"

#include <QGraphicsGridLayout>
#include <QDebug>

#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

RoamingContainer::RoamingContainer(DuiWidget *parent) :
        DuiContainer(DcpNetwork::RoamingTitleText, parent),        
        roamingUpdatesWidget(NULL),
        roamingButton(NULL),
        roamingUpdatesButton(NULL),        
        landscapeLayoutPolicy(NULL),
        portraitLayoutPolicy(NULL)
{
    qDebug() << Q_FUNC_INFO;
    // roaming button
    roamingButton = new DuiButton();
    roamingButton->setCheckable(true);
    roamingButton->setObjectName("basicNetworkButton");
    connect(roamingButton, SIGNAL(toggled(bool)), this, SLOT(toggleRoamingUpdates(bool)));
    connect(roamingButton, SIGNAL(toggled(bool)), this, SIGNAL(roamingToggled(bool)));

    // roaming updates button
    roamingUpdatesButton = new DuiButton();
    roamingUpdatesButton->setCheckable(true);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    connect(roamingUpdatesButton, SIGNAL(toggled(bool)), this, SIGNAL(roamingUpdatesToggled(bool)));

    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    setLayout();
}

RoamingContainer::~RoamingContainer()
{
    qDebug() << Q_FUNC_INFO;
}

void RoamingContainer::setLayout()
{
    qDebug() << Q_FUNC_INFO;
    // roamingLayout
    QGraphicsWidget *roamingWidget = new QGraphicsWidget;
    QGraphicsGridLayout *roamingLayout = new QGraphicsGridLayout(roamingWidget);
    DuiLabel *roamingLabel = new DuiLabel(DcpNetwork::RoamingText);
    roamingLabel->setObjectName("networkLabel");
    roamingLayout->addItem(roamingLabel, 0, 0, 1, 1, Qt::AlignLeft);
    roamingLayout->addItem(roamingButton, 0, 1, 1, 1, Qt::AlignRight);
    roamingLayout->setPreferredWidth(1);

    // roamingUpdatesLayout
    roamingUpdatesWidget = new QGraphicsWidget;
    QGraphicsGridLayout *roamingUpdatesLayout = new QGraphicsGridLayout(roamingUpdatesWidget);
    DuiLabel *roamingUpdatesLabel = new DuiLabel(DcpNetwork::RoamingUpdatesText);
    roamingUpdatesLabel->setObjectName("networkLabel");
    roamingUpdatesLayout->addItem(roamingUpdatesLabel, 0, 0, 1, 1, Qt::AlignLeft);
    roamingUpdatesLayout->addItem(roamingUpdatesButton, 0, 1, 1, 1, Qt::AlignRight);
    roamingUpdatesLayout->setPreferredWidth(1);

    // mainLayout
    DuiLayout *mainLayout = new DuiLayout();
    landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    landscapeLayoutPolicy->addItem(roamingWidget);
    portraitLayoutPolicy->addItem(roamingWidget);
    mainLayout->setLandscapePolicy(landscapeLayoutPolicy);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    centralWidget()->setLayout(mainLayout);
}

void RoamingContainer::initRoamingButton(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    roamingButton->setChecked(toggle);
    toggleRoamingUpdates(toggle);
}

void RoamingContainer::initRoamingUpdatesButton(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    roamingUpdatesButton->setChecked(toggle);
}

void RoamingContainer::toggleRoamingUpdates(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;
    if(toggle) {
        landscapeLayoutPolicy->addItem(roamingUpdatesWidget);
        portraitLayoutPolicy->addItem(roamingUpdatesWidget);
    }
    else {
        landscapeLayoutPolicy->removeItem(roamingUpdatesWidget);
        portraitLayoutPolicy->removeItem(roamingUpdatesWidget);
    }

    if(!toggle && roamingUpdatesButton->isChecked())
        roamingUpdatesButton->toggle();
}
