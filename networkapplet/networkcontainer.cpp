#include "networkcontainer.h"
#include "networklist.h"
#include "networktranslation.h"

#include <DuiComboBox>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#include <QGraphicsLinearLayout>
#include <QDebug>

NetworkContainer::NetworkContainer(DuiWidget *parent) :
        DuiContainer(DcpNetwork::NetworkTitleText, parent),
        modeComboBox(NULL),
        selComboBox(NULL),
        networkList(NULL),
        operatorWidget(NULL),
        infoLabel(NULL)
{    
    modeComboBox = new DuiComboBox();
    connect(modeComboBox, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkModeChanged(QString)));
    selComboBox = new DuiComboBox();
    connect(selComboBox, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkSelectionChanged(QString)));
    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    networkList = new NetworkList();
    connect(networkList, SIGNAL(availableOperatorSelected(int)), this, SIGNAL(availableOperatorSelected(int)));
    setLayout();
}

NetworkContainer::~NetworkContainer()
{
    delete networkList;
    networkList = NULL;
}

void NetworkContainer::setLayout()
{    
    // create labels
    DuiLabel *modeLabel = new DuiLabel(DcpNetwork::NetworkModeText);
    modeLabel->setObjectName("networkLabel");
    DuiLabel *selLabel = new DuiLabel(DcpNetwork::NetworkSelectionText);
    selLabel->setObjectName("networkLabel");
    infoLabel = new DuiLabel();
    infoLabel->setObjectName("networkLabel");   

    // create proxy widget for mode combo box
    QGraphicsWidget *modeWidget = comboBoxWidget(modeLabel, modeComboBox);

    // create proxy widget for selection combo box
    QGraphicsWidget *selWidget = comboBoxWidget(selLabel, selComboBox);

    // create the proxy widget for both combo boxes
    QGraphicsWidget *cbWidget = new QGraphicsWidget;
    DuiLayout *cbLayout = new DuiLayout(cbWidget);
    cbLayout->setContentsMargins(0, 0, 0, 0);
    DuiLinearLayoutPolicy *cbLandscapeLayoutPolicy = new DuiLinearLayoutPolicy(cbLayout, Qt::Horizontal);
    cbLayout->setLandscapePolicy(cbLandscapeLayoutPolicy);
    DuiLinearLayoutPolicy *cbPortraitLayoutPolicy = new DuiLinearLayoutPolicy(cbLayout, Qt::Vertical);
    cbLayout->setPortraitPolicy(cbPortraitLayoutPolicy);

    cbLandscapeLayoutPolicy->addItem(modeWidget, Qt::AlignLeft);
    cbLandscapeLayoutPolicy->addItem(selWidget, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(modeWidget, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(selWidget, Qt::AlignLeft);

    // create the proxy widget for operators
    operatorWidget = new QGraphicsWidget;
    QGraphicsLinearLayout *operatorLayout = new QGraphicsLinearLayout(Qt::Vertical, operatorWidget);
    operatorLayout->setContentsMargins(0, 0, 0, 0);
    operatorLayout->addItem(infoLabel);
    operatorLayout->addItem(networkList);

    // create the main layout
    DuiLayout *layout = new DuiLayout();
    layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(cbWidget, Qt::AlignLeft);

    centralWidget()->setLayout(layout);    
}

QGraphicsWidget* NetworkContainer::comboBoxWidget(DuiLabel *label, DuiComboBox *comboBox)
{
    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setPreferredWidth(1);
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(label);
    layout->addItem(comboBox);
    return widget;
}

void NetworkContainer::initModeComboBox(const QString &value, const QStringList &values)
{    
    initComboBox(modeComboBox, value, values);
}    

void NetworkContainer::initSelectionComboBox(const QString &value, const QStringList &values)
{    
    initComboBox(selComboBox, value, values);
}

void NetworkContainer::initComboBox(DuiComboBox *cb, const QString &value, const QStringList &values)
{
    cb->addItems(values);
    if(values.contains(value)) {
        cb->blockSignals(true);
        cb->setCurrentIndex(values.indexOf(value));
        cb->blockSignals(false);
    }
}

void NetworkContainer::showAvailableOperators(int selected, const QStringList &operators)
{    
    if(operators.size() > 0) {        
        networkList->insertOperators(selected, operators);
        infoLabel->setText(DcpNetwork::AvailableNetworksText);
    }
    else {
        networkList->removeOperators();
        infoLabel->setText(DcpNetwork::NoAvailableNetworksText);
    }
    toggleOperatorWidget(true);
}

void NetworkContainer::hideAvailableOperators()
{
    networkList->removeOperators();    
    toggleOperatorWidget(false);
}

void NetworkContainer::toggleOperatorWidget(bool toggle)
{
    if(toggle) {
        if(layoutPolicy->indexOf(operatorWidget) == -1)
            layoutPolicy->addItem(operatorWidget, Qt::AlignLeft);
    }
    else
        layoutPolicy->removeItem(operatorWidget);
}

void NetworkContainer::setDefaultSelection(const QString &value)
{
    for(int i=0; i<selComboBox->count(); ++i) {
        if(selComboBox->itemText(i) == value) {
            selComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void NetworkContainer::operatorSelectionFailed()
{
    networkList->removeSelection();
}

