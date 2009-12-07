#include "callcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiComboBox>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiButton>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QDebug>

CallContainer::CallContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::CallText, parent),
        lp(NULL),
        pp(NULL)
{
    // layout & policies
    DuiLayout* layout = new DuiLayout();
    lp = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layout->setLandscapePolicy(lp); // ownership transferred
    pp = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(pp); // ownership transferred

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
    //lp->addItemAtPosition(sendCallerIdWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    //lp->addItemAtPosition(callWaitingWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // portrait policy
    pp->setSpacing(5);
    pp->addItem(sendCallerIdWidget);
    pp->addItem(callWaitingWidget);
//    pp->addItemAtPosition(sendCallerIdWidget, 0, 0, Qt::AlignCenter);
//    pp->addItemAtPosition(callWaitingWidget, 1, 0, Qt::AlignCenter);

    // connect signals
    connect(sendCallerIdComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendCallerIdSelected(int)));
    connect(callWaitingButton, SIGNAL(toggled(bool)), this, SLOT(callWaitingToggled(bool)));

    // layout
    centralWidget()->setLayout(layout);
}

CallContainer::~CallContainer()
{
}

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

void CallContainer::requestFailed(DcpCallAndSim::Data data)
{
    qDebug() << Q_FUNC_INFO << data;

    switch (data) {
    case DcpCallAndSim::CallerIdSendingData:
        sendCallerIdComboBox->blockSignals(true);
        sendCallerIdComboBox->setCurrentIndex(0);
        sendCallerIdComboBox->blockSignals(false);
        break;
    case DcpCallAndSim::CallWaitingData:
        callWaitingButton->blockSignals(true);
        callWaitingButton->setChecked(false);
        callWaitingButton->blockSignals(false);
        break;
    default:
        break;
    }
}

void CallContainer::sendCallerIdSelected(int index)
{
    qDebug() << Q_FUNC_INFO << index;

    emit sendCallerIdChanged(index);
}

void CallContainer::callWaitingToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    emit callWaitingChanged(checked);
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

