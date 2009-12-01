#include "offlinecontainer.h"

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiLinearLayoutPolicy>

namespace {
    const QString SwitchOfflineOn = trid("NOT_DEFINED", "Switch to Offline mode");
    const QString SwitchOfflineOff = trid("NOT_DEFINED", "Exit Offline mode");
}

OfflineContainer::OfflineContainer(bool state, DuiWidget *parent) :
        DuiContainer(parent),
        offlineButton(NULL)
{    
    offlineButton = new DuiButton();
    offlineButton->setObjectName("offlineButton");
    offlineButton->setText(state ? SwitchOfflineOff : SwitchOfflineOn);

    connect(offlineButton, SIGNAL(clicked()), this, SLOT(offlineButtonClicked()));
    setLayout();
}

OfflineContainer::~OfflineContainer()
{
}    

void OfflineContainer::offlineButtonClicked()
{
    QString text = offlineButton->text();
    emit toggled(text == SwitchOfflineOn);
    offlineButton->setText(text == SwitchOfflineOn ? SwitchOfflineOff : SwitchOfflineOn);
}

bool OfflineContainer::offlineMode()
{
    return (offlineButton->text() == SwitchOfflineOff);
}

void OfflineContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();    
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layoutPolicy->addItem(offlineButton, Qt::AlignCenter);
    layoutPolicy->insertStretch(1, 1);
    layoutPolicy->insertStretch(0, 1);

    //layoutPolicy->insertStretch(0, 1);
    centralWidget()->setLayout(layout);
}

