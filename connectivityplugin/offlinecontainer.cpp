#include "offlinecontainer.h"

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiLinearLayoutPolicy>

OfflineContainer::OfflineContainer(bool state, DuiWidget *parent) :
        DuiContainer(parent),
        toggleButton(NULL)
{    
    toggleButton = new DuiButton();
    toggleButton->setCheckable(true);
    toggleButton->setChecked(state);
    toggleButton->setObjectName("connectivityButton");
    connect(toggleButton, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));    
    setLayout();
}

OfflineContainer::~OfflineContainer()
{
}    

bool OfflineContainer::offlineMode()
{
    return toggleButton->isChecked();
}

void OfflineContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiLabel *headerLabel = new DuiLabel(trid("qtn_offline_offline", "Offline mode"));
    headerLabel->setObjectName("connectivityLabel1");
    layoutPolicy->addItem(headerLabel, Qt::AlignLeft);
    layoutPolicy->addItem(toggleButton, Qt::AlignRight);
    centralWidget()->setLayout(layout);
}

