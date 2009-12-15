#include "roamingcontainer.h"
#include "networktranslation.h"

#include <QDebug>

#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

RoamingContainer::RoamingContainer(RoamingContainer::RoamingState state, bool roamingUpdatesState, DuiWidget *parent) :
        DuiContainer(DcpNetwork::RoamingTitleText, parent)
{
    qDebug() << Q_FUNC_INFO;

    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    setLayout(state, roamingUpdatesState);
}

RoamingContainer::~RoamingContainer()
{
    qDebug() << Q_FUNC_INFO;

    lowerWidget->deleteLater();
    lowerWidget = NULL;
}

void RoamingContainer::setLayout(RoamingContainer::RoamingState state, bool roamingUpdatesState)
{
    qDebug() << Q_FUNC_INFO;

    // create button group and buttons
    buttons = new DuiButtonGroup();
    connect(buttons, SIGNAL(buttonClicked(DuiButton*)), this, SLOT(buttonClicked(DuiButton*)));
    alwaysAskButton = createButton(DcpNetwork::RoamingAlwaysAskText, QString("networkRoamingButton"),
                                   state == RoamingContainer::AlwaysAsk);
    alwaysAllowButton = createButton(DcpNetwork::RoamingAlwaysAllowText, QString("networkRoamingButton"),
                                     state == RoamingContainer::AlwaysAllow);
    buttons->addButton(alwaysAskButton);
    buttons->addButton(alwaysAllowButton);

    // create label
    DuiLabel *updatesLabel = new DuiLabel(DcpNetwork::RoamingUpdatesText);
    updatesLabel->setObjectName("networkLabel");

    // create toggle button
    updatesToggleButton = createButton(QString(""), QString("networkToggleButton"),
                                       (state == RoamingContainer::AlwaysAllow && roamingUpdatesState));
    connect(updatesToggleButton, SIGNAL(toggled(bool)), this, SIGNAL(roamingUpdatesToggled(bool)));

    // upper widget
    QGraphicsWidget *upperWidget = new QGraphicsWidget;
    DuiLayout *upperLayout = new DuiLayout(upperWidget);

    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(upperLayout, Qt::Horizontal);
    landscapeLayoutPolicy->addItem(alwaysAskButton, Qt::AlignLeft);
    landscapeLayoutPolicy->addItem(alwaysAllowButton, Qt::AlignLeft);
    upperLayout->setLandscapePolicy(landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(upperLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem(alwaysAskButton, Qt::AlignLeft);
    portraitLayoutPolicy->addItem(alwaysAllowButton, Qt::AlignLeft);
    upperLayout->setPortraitPolicy(portraitLayoutPolicy);

    // lower widget
    lowerWidget = new QGraphicsWidget;
    DuiLayout *lowerLayout = new DuiLayout(lowerWidget);
    DuiLinearLayoutPolicy *lowerLayoutPolicy = new DuiLinearLayoutPolicy(lowerLayout, Qt::Horizontal);
    lowerLayoutPolicy->addItem(updatesLabel, Qt::AlignLeft);
    lowerLayoutPolicy->addItem(updatesToggleButton, Qt::AlignRight);

    // main layout
    DuiLayout *mainLayout = new DuiLayout();
    mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(upperWidget, Qt::AlignLeft);
    toggleLowerWidget(state == RoamingContainer::AlwaysAllow);
    centralWidget()->setLayout(mainLayout);
}

DuiButton* RoamingContainer::createButton(const QString &text, const QString &name, bool checked)
{
    qDebug() << Q_FUNC_INFO;

    DuiButton* button = new DuiButton(text);
    button->setCheckable(true);
    button->setChecked(checked);
    button->setObjectName(name);
    return button;
}

void RoamingContainer::buttonClicked(DuiButton *button)
{
    qDebug() << Q_FUNC_INFO;

    if(button->text() == DcpNetwork::RoamingAlwaysAskText) {
        toggleLowerWidget(false);
    } else {
        toggleLowerWidget(true);
    }
    emit roamingStateChanged(button->text() == DcpNetwork::RoamingAlwaysAskText ?
                             RoamingContainer::AlwaysAsk : RoamingContainer::AlwaysAllow);
}

void RoamingContainer::toggleLowerWidget(bool toggle)
{
    qDebug() << Q_FUNC_INFO;

    if(toggle) {
        if(mainLayoutPolicy->indexOf(lowerWidget) == -1) {
            mainLayoutPolicy->addItem(lowerWidget, Qt::AlignLeft);
        }
    } else {        
        mainLayoutPolicy->removeItem(lowerWidget);
        updatesToggleButton->setChecked(false);
    }
}

void RoamingContainer::changeRoamingState(RoamingContainer::RoamingState state)
{
    qDebug() << Q_FUNC_INFO;

    buttons->blockSignals(true);
    switch(state) {
        case RoamingContainer::AlwaysAsk:            
            alwaysAskButton->setChecked(true);
            toggleLowerWidget(false);            
            break;
        case RoamingContainer::AlwaysAllow:            
            alwaysAllowButton->setChecked(true);
            toggleLowerWidget(true);            
            break;        
        default:
            break;
    }
    buttons->blockSignals(false);
}

void RoamingContainer::toggleRoamingUpdates(bool toggle)
{        
    if(toggle) {
        buttons->blockSignals(true);
        alwaysAllowButton->setChecked(true);
        buttons->blockSignals(false);
    }

    alwaysAllowButton->blockSignals(true);
    updatesToggleButton->setChecked(toggle);
    alwaysAllowButton->blockSignals(false);
}
