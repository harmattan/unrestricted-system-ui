#include "roamingcontainer.h"
#include "networktranslation.h"

#include <QDebug>

#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

RoamingContainer::RoamingContainer(DuiWidget *parent) :
        DuiContainer(DcpNetwork::RoamingTitleText, parent),
        roamingButton(NULL),
        roamingUpdatesButton(NULL)
{
    roamingButton = new DuiButton();
    roamingButton->setCheckable(true);
    roamingButton->setObjectName("basicNetworkButton");
    roamingUpdatesButton = new DuiButton();
    roamingUpdatesButton->setCheckable(true);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    //roamingUpdatesButton->setVisible(false);
    roamingUpdatesLabel = new DuiLabel(DcpNetwork::RoamingUpdatesText);
    roamingUpdatesLabel->setObjectName("networkLabel");
    //roamingUpdatesLabel->setVisible(false);
    connect(roamingButton, SIGNAL(toggled(bool)), this, SLOT(toggleRoamingUpdates(bool)));
    connect(roamingButton, SIGNAL(toggled(bool)), this, SIGNAL(roamingToggled(bool)));
    connect(roamingUpdatesButton, SIGNAL(toggled(bool)), this, SIGNAL(roamingUpdatesToggled(bool)));
    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    setLayout();
}

RoamingContainer::~RoamingContainer()
{
}

void RoamingContainer::setLayout()
{       
    // roamingLayout
    DuiLayout *roamingLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingLayoutPolicy = new DuiLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    DuiLabel *roamingLabel = new DuiLabel(DcpNetwork::RoamingText);
    roamingLabel->setObjectName("networkLabel");
    roamingLayoutPolicy->addItem(roamingLabel, Qt::AlignLeft);
    roamingLayoutPolicy->addItem(roamingButton, Qt::AlignRight);

    // roamingUpdatesLayout
    DuiLayout *roamingUpdatesLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingUpdatesLayoutPolicy = new DuiLinearLayoutPolicy(roamingUpdatesLayout, Qt::Horizontal);
    roamingUpdatesLayoutPolicy->addItem(roamingUpdatesLabel, Qt::AlignLeft);
    roamingUpdatesLayoutPolicy->addItem(roamingUpdatesButton, Qt::AlignRight);

    // mainLayout
    DuiLayout *mainLayout = new DuiLayout();
    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    landscapeLayoutPolicy->addItem(roamingLayout);
    landscapeLayoutPolicy->addItem(roamingUpdatesLayout);
    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem(roamingLayout);
    portraitLayoutPolicy->addItem(roamingUpdatesLayout);
    mainLayout->setLandscapePolicy(landscapeLayoutPolicy);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    centralWidget()->setLayout(mainLayout);
}

void RoamingContainer::initRoamingButton(bool toggle)
{
    roamingButton->setChecked(toggle);
    toggleRoamingUpdates(toggle);
}

void RoamingContainer::initRoamingUpdatesButton(bool toggle)
{
    roamingUpdatesButton->setChecked(toggle);
}

void RoamingContainer::toggleRoamingUpdates(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;
    roamingUpdatesButton->setVisible(toggle);
    roamingUpdatesLabel->setVisible(toggle);
    if(!toggle && roamingUpdatesButton->isChecked())
        roamingUpdatesButton->toggle();
}
