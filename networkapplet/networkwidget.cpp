#include "networkwidget.h"
#include "networktranslation.h"
#include "networkdbusinterface.h"
#include "dcpnetwork.h"

#include <QDebug>
#include <QTimer>

#include <DuiButton>
#include <DuiComboBox>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiStylableWidget>
#include <DuiSceneManager>
#include <DuiButtonGroup>
#include <DuiWidgetController>
#include <QSignalMapper>
#include <QSizePolicy>

NetworkWidget::NetworkWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpNetwork::None);
    initWidget();
}

NetworkWidget::~NetworkWidget()
{
     networkIf->networkAppletClosing();
     delete networkIf;
     networkIf = NULL;
}

void NetworkWidget::initWidget()
{    
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    //create dbus if
    networkIf = new NetworkDBusInterface();

    availableNetworksButtonGroup = new DuiButtonGroup(this);

    // catch network If actions    
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(initPhoneNetworkButton(bool)));
    connect(networkIf, SIGNAL(roamingValueReceived(bool)), this, SLOT(initRoamingButton(bool)));
    connect(networkIf, SIGNAL(roamingUpdatesValueReceived(bool)), this, SLOT(initRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(networkModeValuesReceived(int, QStringList)), this, SLOT(initNetworkModeComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(networkSelectionValuesReceived(int, QStringList)), this, SLOT(initNetworkSelectionComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(availableNetworksReceived(int, QStringList)), this, SLOT(toggleAvailableNetworks(int, QStringList)));    



    /*
     * phoneNetworkLabel and phoneNetworkButton
     */
    DuiLabel *phoneNetworkLabel = new DuiLabel(DcpNetwork::PhoneNetworkText);
    phoneNetworkButton = new DuiButton(this);
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("basicNetworkButton");    

    /*
     * roamingContainer
     */
    roamingButton = new DuiButton(this);
    roamingButton->setObjectName("basicNetworkButton");
    roamingButton->setCheckable(true);
    roamingUpdatesButton = new DuiButton(this);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    roamingUpdatesButton->setCheckable(true);        

    widgets << new DuiLabel(DcpNetwork::RoamingButtonText) << roamingButton;
    alignments.insert(roamingButton, Qt::AlignRight);
    DuiStylableWidget *roamingLeftLayoutWidget = createStylableWidget(Qt::Horizontal, QString("networkLayoutWidget2"));

    clearWidgetLists();
    widgets << new DuiLabel(DcpNetwork::RoamingUpdatesButtonText) << roamingUpdatesButton;
    alignments.insert(roamingUpdatesButton, Qt::AlignRight);
    roamingRightLayoutWidget = createStylableWidget(Qt::Horizontal, QString(""));

    DuiLayout *roamingLayout = new DuiLayout();
    clearWidgetLists();
    widgets << roamingLeftLayoutWidget << roamingRightLayoutWidget;
    roamingLandscapeLayoutPolicy = createLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    roamingLayout->setLandscapePolicy(roamingLandscapeLayoutPolicy);
    roamingPortraitLayoutPolicy = createLinearLayoutPolicy(roamingLayout, Qt::Vertical);
    roamingLayout->setPortraitPolicy(roamingPortraitLayoutPolicy);

    networkIf->roamingUpdatesValueRequired();
    networkIf->roamingValueRequired();

    roamingContainer = createContainer(roamingLayout, DcpNetwork::RoamingText, true);

    /*
     * dataCounterButton
     */
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText, this);
    dataCounterButton->setObjectName("dataCounterButton");

    /*
     * networkContainer
     */
    networkModeComboBox = new DuiComboBox(this);    
    networkIf->networkModeValuesRequired();    
    networkSelectionComboBox = new DuiComboBox(this);    
    networkIf->networkSelectionValuesRequired();    
    availableNetworksLabel = NULL; //init to null to prevent from showing behind the other widgets
    noAvailableNetworksLabel = NULL; //init to null to prevent from showing behind the other widgets

    DuiLayout *networkComboBoxLayout = new DuiLayout();    
    DuiGridLayoutPolicy *networkComboBoxLayoutPolicy = new DuiGridLayoutPolicy(networkComboBoxLayout);    
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkModeText), 0, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkSelectionText), 0, 1);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkModeComboBox, 1, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkSelectionComboBox, 1, 1);    

    DuiLayout *networkLayout = new DuiLayout();
    networkLayoutPolicy = new DuiLinearLayoutPolicy(networkLayout, Qt::Vertical);
    networkLayoutPolicy->addItem(networkComboBoxLayout, Qt::AlignLeft);

    networkContainer = createContainer(networkLayout, DcpNetwork::NetworkText, false);

    /*
     * contentContainer
     */
    DuiLayout *contentLayout = new DuiLayout();

    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkLabel, 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);    
    contentItems << roamingContainer << dataCounterButton << networkContainer;

    DuiContainer *contentContainer = createContainer(contentLayout);    

    networkIf->phoneNetworkValueRequired();

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);
    clearWidgetLists();
    widgets << contentContainer;
    DuiLinearLayoutPolicy *mainLayoutPolicy = createLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    Q_UNUSED(mainLayoutPolicy);

    // catch user actions
    signalMapper = new QSignalMapper(this);    
    connect(phoneNetworkButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(roamingButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(roamingUpdatesButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(dataCounterButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    connect(roamingContainer, SIGNAL(headerClicked()), roamingContainer, SLOT(toggleExpand()));
    connect(networkContainer, SIGNAL(headerClicked()), networkContainer, SLOT(toggleExpand()));

    signalMapper->setMapping(phoneNetworkButton, DcpNetwork::PhoneNetworkText);
    signalMapper->setMapping(roamingButton, DcpNetwork::RoamingButtonText);
    signalMapper->setMapping(roamingUpdatesButton, DcpNetwork::RoamingUpdatesButtonText);
    signalMapper->setMapping(dataCounterButton, DcpNetwork::DataCounterText);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonPressed(QString)));
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    this->setLayout(mainLayout);    
}

void NetworkWidget::clearWidgetLists()
{
    widgets.clear();
    alignments.clear();
}

DuiStylableWidget* NetworkWidget::createStylableWidget(Qt::Orientation policyOrientation, const QString &widgetObjectName, int policySpacing)
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = createLinearLayoutPolicy(layout, policyOrientation, policySpacing);
    Q_UNUSED(layoutPolicy);
    DuiStylableWidget *stylableWidget = new DuiStylableWidget(this);
    stylableWidget->setLayout(layout);
    stylableWidget->setObjectName(widgetObjectName);
    return stylableWidget;
}

DuiLinearLayoutPolicy* NetworkWidget::createLinearLayoutPolicy(DuiLayout *layout, Qt::Orientation policyOrientation, int policySpacing)
{
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, policyOrientation);
    for(int i=0; i<widgets.size(); ++i)
        layoutPolicy->addItem(widgets.at(i), (alignments.value(widgets.at(i)) == 0 ? Qt::AlignLeft : alignments.value(widgets.at(i))));
    layoutPolicy->setSpacing(policySpacing);
    return layoutPolicy;
}

DuiContainer* NetworkWidget::createContainer(DuiLayout *layout, const QString &title, bool expandable)
{
    DuiStylableWidget *stylableWidget = new DuiStylableWidget(this);
    stylableWidget->setLayout(layout);
    DuiContainer *container = new DuiContainer(this);
    container->setCentralWidget(stylableWidget);
    if(!title.isEmpty())
        container->setTitle(title);
    container->setExpand(expandable);
    return container;
}


void NetworkWidget::initPhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleSettings(toggle);
}

void NetworkWidget::initRoamingButton(bool toggle)
{
    roamingButton->setChecked(toggle);
    toggleRoamingUpdatesButton(toggle);
}

void NetworkWidget::initRoamingUpdatesButton(bool toggle)
{
    roamingUpdatesButton->setChecked(toggle);
}

void NetworkWidget::initNetworkModeComboBox(int selected, const QStringList &values)
{    
    networkModeComboBox->addItems(values);
    if (selected != -1) {
        disconnect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
        networkModeComboBox->setCurrentIndex(selected);
        connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    }
}

void NetworkWidget::initNetworkSelectionComboBox(int selected, const QStringList &values)
{    
    networkSelectionComboBox->addItems(values);
    if (selected != -1) {
        disconnect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
        networkSelectionComboBox->setCurrentIndex(selected);
        connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    }
}

void NetworkWidget::toggleSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleSettings(" << toggle << ")";
    if(toggle) {
        if(contentLayoutPolicy->indexOf(contentItems.at(0)) == -1) {
            for(int i=0; i<contentItems.size(); ++i)
                contentLayoutPolicy->addItemAtPosition(contentItems.at(i), i+1, 0, 1, 2);            
            contentLayoutPolicy->setSpacing(10);
        }
    }
    else {
        if(contentLayoutPolicy->indexOf(contentItems.at(0)) != -1) {
            for(int i=0; i<contentItems.size(); ++i)
                contentLayoutPolicy->removeItem(contentItems.at(i));
            contentLayoutPolicy->setSpacing(0);
        }
    }
}

void NetworkWidget::toggleRoamingUpdatesButton(bool toggle)
{
    if(toggle) {        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) == -1) {
            roamingLandscapeLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
            roamingPortraitLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
        }
    }
    else {       
        if(roamingUpdatesButton->isChecked()) {
            roamingUpdatesButton->setChecked(false);
            networkIf->setRoamingUpdatesValue(false);
        }        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) != -1) {
            roamingLandscapeLayoutPolicy->removeItem(roamingRightLayoutWidget);
            roamingPortraitLayoutPolicy->removeItem(roamingRightLayoutWidget);            
        }        
    }
}



void NetworkWidget::buttonPressed(const QString &text)
{   
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point    

    if(text == DcpNetwork::PhoneNetworkText) {
        bool checked = !phoneNetworkButton->isChecked();
        networkIf->setPhoneNetworkValue(checked);
        toggleSettings(checked);
    }

    else if(text == DcpNetwork::RoamingButtonText) {
        bool checked = !roamingButton->isChecked();

        networkIf->setRoamingValue(checked);
        toggleRoamingUpdatesButton(checked);
    }

    else if(text == DcpNetwork::RoamingUpdatesButtonText)
        networkIf->setRoamingUpdatesValue(!roamingUpdatesButton->isChecked());

    else if(text == DcpNetwork::DataCounterText)
        qDebug() << "Show dialog";
}

void NetworkWidget::toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle)
{        
    if(toggle) {
        if(availableNetworksButtonGroup->buttons().size() == 0) { //no buttons on the screen
            if(networks.size() > 0) { //some networks available
                if(availableNetworksLabel == NULL)
                    availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
                networkLayoutPolicy->addItem(availableNetworksLabel, Qt::AlignLeft);
                for(int i=0; i<networks.size(); ++i) {
                    DuiButton *button = new DuiButton(networks.at(i), this);
                    button->setCheckable(true);
                    if(i==selected)
                        button->setChecked(true);
                    availableNetworksButtonGroup->addButton(button);
                    networkLayoutPolicy->addItem(button, Qt::AlignLeft);
                }
                connect(availableNetworksButtonGroup, SIGNAL(buttonPressed(DuiButton*)),
                        this, SLOT(availableNetworksButtonPressed(DuiButton*)));
            }
            else {
                if(noAvailableNetworksLabel == NULL)
                    noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);
                networkLayoutPolicy->addItem(noAvailableNetworksLabel, Qt::AlignLeft);
            }
        }       
    }
    else {
        if(availableNetworksButtonGroup->buttons().size() > 0) {
            if(availableNetworksLabel == NULL)
                availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
            networkLayoutPolicy->removeItem(availableNetworksLabel);
            QList<DuiButton *> buttons = availableNetworksButtonGroup->buttons();
            while(!buttons.isEmpty()) {
                DuiButton *button = buttons.takeLast();
                availableNetworksButtonGroup->removeButton(button);
                networkLayoutPolicy->removeItem(button);
                delete button;
                button = NULL;
            }            
        }
        else {
            if(noAvailableNetworksLabel == NULL)
                noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);
            networkLayoutPolicy->removeItem(noAvailableNetworksLabel);
        }
    }

}

void NetworkWidget::availableNetworksButtonPressed(DuiButton *button)
{
    networkIf->setNetworkSelectionValue(button->text());
}
