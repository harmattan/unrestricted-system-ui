#include "callcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiComboBox>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiButton>
#include <DuiNotification>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QDebug>

CallContainer::CallContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::CallText, parent),
        lp(NULL),
        pp(NULL)
{
    // layout & policies
    DuiLayout* layout = new DuiLayout;
    lp = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layout->setLandscapePolicy(lp);
    pp = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(pp);

    // send caller id widget
    sendCallerIdComboBox = new DuiComboBox();
    sendCallerIdComboBox->addItem(DcpCallAndSim::LetNetworkChooseText);
    sendCallerIdComboBox->addItem(DcpCallAndSim::YesText);
    sendCallerIdComboBox->addItem(DcpCallAndSim::NoText);
    sendCallerIdComboBox->setIconVisible(false);
    sendCallerIdComboBox->setCurrentIndex(0);

    QGraphicsWidget *sendCallerIdWidget = new QGraphicsWidget;
    sendCallerIdWidget->setPreferredWidth(1);
    QGraphicsLinearLayout* sendCallerIdLayout = new QGraphicsLinearLayout(Qt::Vertical, sendCallerIdWidget);
    sendCallerIdLayout->setContentsMargins(0, 0, 0, 0);
    sendCallerIdLayout->addItem(createLabel(DcpCallAndSim::SendCallerIdText));
    sendCallerIdLayout->addItem(sendCallerIdComboBox);

    // call waiting checkbox widget
    QGraphicsWidget* callWaitingWidget = createCheckBox(DcpCallAndSim::CallWaitingText, callWaitingButton);

    // landscape policy
    lp->setSpacing(5);
    lp->addItem(sendCallerIdWidget);
    lp->addItem(callWaitingWidget);

    // portrait policy
    pp->setSpacing(5);
    pp->addItem(sendCallerIdWidget);
    pp->addItem(callWaitingWidget);

    // connect signals
    connect(sendCallerIdComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendCallerIdSelected(int)));
    connect(callWaitingButton, SIGNAL(toggled(bool)), this, SLOT(callWaitingToggled(bool)));

    callWaiting = new CallWaiting(this);

    connect(callWaiting, SIGNAL(waitingActivateComplete(CallWaiting::WaitingError)),
            this, SLOT(waitingActivateComplete(CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCheckComplete(bool, CallWaiting::WaitingError)),
            this, SLOT(waitingCheckComplete(bool, CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCancelComplete(CallWaiting::WaitingError)),
            this, SLOT(waitingCancelComplete(CallWaiting::WaitingError)));

    callWaiting->waitingCheck();

    // layout
    centralWidget()->setLayout(layout);
}

CallContainer::~CallContainer()
{
}
/*
void CallContainer::setSendCallerId(int value)
{
    Q_ASSERT(value >= 0 && value < 3);

    if (sendCallerIdComboBox->currentIndex() != value) {
        sendCallerIdComboBox->setCurrentIndex(value);
    }
}

void CallContainer::setCallWaiting(bool enabled)
{
    if (callWaitingButton->isChecked() != enabled) {
        callWaitingButton->setChecked(enabled);
    }
}
*/

void CallContainer::waitingActivateComplete(CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallWaiting::NoError) {
//        DuiNotification notification("", "", "waitingActivate failed");
    }
}

void CallContainer::waitingCancelComplete(CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallWaiting::NoError) {
//        DuiNotification notification("", "", "waitingCancel failed");
    }

    callWaitingButton->setChecked(false);
}

void CallContainer::waitingCheckComplete(bool active, CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << active << error;

    if (error != CallWaiting::NoError) {
//        DuiNotification notification("", "", "waitingCheck failed");
    }

    callWaitingButton->setChecked(active);
}

void CallContainer::sendCallerIdSelected(int index)
{
    qDebug() << Q_FUNC_INFO << index;
}

void CallContainer::callWaitingToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    if (checked) {
        callWaiting->waitingActivate();
    } else {
        callWaiting->waitingCancel();
    }
}

DuiLabel* CallContainer::createLabel(const QString& text)
{
    DuiLabel* label = new DuiLabel(text);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return label;
}

QGraphicsWidget* CallContainer::createCheckBox(const QString& text, DuiButton*& button)
{
    button = new DuiButton();
    button->setObjectName("checkBoxButton");
    button->setCheckable(true);

    DuiLabel* label = createLabel(text);

    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setPreferredWidth(1);
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal, widget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addItem(label);
    layout->setAlignment(label, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addItem(button);
    layout->setAlignment(button, Qt::AlignRight | Qt::AlignVCenter);

    return widget;
}

