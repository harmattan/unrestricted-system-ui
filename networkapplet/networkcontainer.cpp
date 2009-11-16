#include "networkcontainer.h"
#include "networklist.h"
#include "networktranslation.h"

#include <DuiComboBox>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#include <QDebug>

NetworkContainer::NetworkContainer(DuiWidget *parent) :
        DuiContainer(DcpNetwork::NetworkTitleText, parent),
        modeComboBox(NULL),
        selectionComboBox(NULL),
        infoLabel(NULL),
        modeLabel(NULL),
        selectionLabel(NULL),        
        networkList(NULL)
{    
    modeComboBox = new DuiComboBox();
    selectionComboBox = new DuiComboBox();
    modeLabel = new DuiLabel(DcpNetwork::NetworkModeText);
    modeLabel->setObjectName("networkLabel");
    selectionLabel = new DuiLabel(DcpNetwork::NetworkSelectionText);
    selectionLabel->setObjectName("networkLabel");
    toggleComboBoxSignalConnection(modeComboBox);
    toggleComboBoxSignalConnection(selectionComboBox);
    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    setLayout();
}

NetworkContainer::~NetworkContainer()
{
    delete networkList;
    networkList = NULL;
}

void NetworkContainer::setLayout()
{
    // create the layout for combo boxes
    DuiLayout *cbLayout = new DuiLayout();

    DuiGridLayoutPolicy *cbLandscapeLayoutPolicy = new DuiGridLayoutPolicy(cbLayout);
    cbLandscapeLayoutPolicy->addItemAtPosition(modeLabel, 0, 0);
    cbLandscapeLayoutPolicy->addItemAtPosition(selectionLabel, 0, 1);
    cbLandscapeLayoutPolicy->addItemAtPosition(modeComboBox, 1, 0);
    cbLandscapeLayoutPolicy->addItemAtPosition(selectionComboBox, 1, 1);
    cbLayout->setLandscapePolicy(cbLandscapeLayoutPolicy);

    DuiLinearLayoutPolicy *cbPortraitLayoutPolicy = new DuiLinearLayoutPolicy(cbLayout, Qt::Vertical);
    cbPortraitLayoutPolicy->addItem(modeLabel, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(modeComboBox, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(selectionLabel, Qt::AlignLeft);    
    cbPortraitLayoutPolicy->addItem(selectionComboBox, Qt::AlignLeft);
    cbLayout->setPortraitPolicy(cbPortraitLayoutPolicy);

    // create the main layout
    DuiLayout *layout = new DuiLayout();
    layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(cbLayout, Qt::AlignLeft);

    centralWidget()->setLayout(layout);
}

void NetworkContainer::initModeComboBox(const QString &value, const QStringList &values)
{    
    initComboBox(modeComboBox, value, values);
}    

void NetworkContainer::initSelectionComboBox(const QString &value, const QStringList &values)
{    
    initComboBox(selectionComboBox, value, values);
}

void NetworkContainer::initComboBox(DuiComboBox *cb, const QString &value, const QStringList &values)
{
    cb->addItems(values);
    if(values.contains(value)) {
        toggleComboBoxSignalConnection(cb, false);
        cb->setCurrentIndex(values.indexOf(value));
        toggleComboBoxSignalConnection(cb);
    }
}

void NetworkContainer::toggleComboBoxSignalConnection(DuiComboBox *cb, bool toggle)
{
    if(toggle) {
        if(cb == modeComboBox)
            connect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkModeChanged(QString)));
        else if(cb == selectionComboBox)
            connect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkSelectionChanged(QString)));
    }
    else {
        if(cb == modeComboBox)
            disconnect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkModeChanged(QString)));
        else if(cb == selectionComboBox)
            disconnect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkSelectionChanged(QString)));
    }
}

void NetworkContainer::toggleAvailableOperators(int selected, const QStringList &operators, bool toggle)
{    
    if(toggle) {
        if(infoLabel == NULL) {
            infoLabel = new DuiLabel();
            infoLabel->setObjectName("networkLabel");
        }
        if(networkList == NULL) {
            networkList = new NetworkList();
            connect(networkList, SIGNAL(availableOperatorSelected(int)), this, SIGNAL(availableOperatorSelected(int)));
        }
        if(networkList->insertOperators(selected, operators)) {
            infoLabel->setText(DcpNetwork::AvailableNetworksText);
            if(layoutPolicy->indexOf(infoLabel) == -1)
                layoutPolicy->addItem(infoLabel, Qt::AlignLeft);
            if(layoutPolicy->indexOf(networkList) == -1)
                layoutPolicy->addItem(networkList, Qt::AlignLeft);
        }
        else {
            infoLabel->setText(DcpNetwork::NoAvailableNetworksText);
            if(layoutPolicy->indexOf(infoLabel) == -1)
                layoutPolicy->addItem(infoLabel, Qt::AlignLeft);
        }
    }
    else {
        if(infoLabel != NULL) {
            layoutPolicy->removeItem(infoLabel);
            delete infoLabel;
            infoLabel = NULL;
        }
        if(networkList != NULL) {
            layoutPolicy->removeItem(networkList);
            delete networkList;
            networkList = NULL;
        }        
    }
}

void NetworkContainer::setDefaultSelection(const QString &value)
{
    for(int i=0; i<selectionComboBox->count(); ++i) {
        if(selectionComboBox->itemText(i) == value) {
            selectionComboBox->setCurrentIndex(i);
            break;
        }
    }
}

bool NetworkContainer::operatorSelected()
{
    if(networkList != NULL)
        if(!networkList->itemSelected())
            return false;
    return true;
}
