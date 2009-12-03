#include "callcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiComboBox>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiButton>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QDebug>

CallContainer::CallContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::CallText, parent),
        sendCallerIdComboBox(NULL),
        callWaitingButton(NULL),
        callFwdButton(NULL),
        numberLabel(NULL),
        numberEdit(NULL),
        pickerButton(NULL),
        fwdNumberWidget(NULL),
        dummyWidget(NULL),
        lp(NULL),
        pp(NULL)
{
    // layout & policies
    DuiLayout* layout = new DuiLayout();
    lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred
    pp = new DuiGridLayoutPolicy(layout);
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

    // call forwarding checkbox widget
    QGraphicsWidget* callFwdWidget = createCheckBox(DcpCallAndSim::CallForwardingText, callFwdButton);

    // call forwarding number widget
    numberLabel = createLabel(DcpCallAndSim::ForwardToText);
    numberEdit = new DuiTextEdit();
    pickerButton = new DuiButton();
    pickerButton->setObjectName("peoplePickerButton");
    pickerButton->setIconID("icon-m-addressbook");

    fwdNumberWidget = new QGraphicsWidget;
    fwdNumberWidget->setPreferredWidth(1);    
    QGraphicsGridLayout* fwdNumberLayout = new QGraphicsGridLayout(fwdNumberWidget);
    fwdNumberLayout->setContentsMargins(0, 0, 0, 0);
    fwdNumberLayout->addItem(numberLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    fwdNumberLayout->addItem(pickerButton, 0, 1, 2, 1, Qt::AlignCenter);
    fwdNumberLayout->addItem(numberEdit, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // dummy placeholder
    dummyWidget = new QGraphicsWidget;
    dummyWidget->setContentsMargins(0, 0, 0, 0);
    dummyWidget->setPreferredWidth(1);

    // landscape policy
    lp->setSpacing(5);    
    lp->addItemAtPosition(sendCallerIdWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callWaitingWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callFwdWidget, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(dummyWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // portrait policy
    pp->setSpacing(5);    
    pp->addItemAtPosition(sendCallerIdWidget, 0, 0, Qt::AlignCenter);
    pp->addItemAtPosition(callWaitingWidget, 1, 0, Qt::AlignCenter);
    pp->addItemAtPosition(callFwdWidget, 2, 0, Qt::AlignCenter);
    pp->addItemAtPosition(dummyWidget, 3, 0, Qt::AlignCenter);

    // connect signals
    connect(sendCallerIdComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendCallerIdSelected(int)));
    connect(callWaitingButton, SIGNAL(toggled(bool)), this, SLOT(callWaitingToggled(bool)));
    connect(callFwdButton, SIGNAL(toggled(bool)), this, SLOT(callForwardingToggled(bool)));
    connect(numberEdit, SIGNAL(lostFocus(Qt::FocusReason)), this, SLOT(numberChanged()));

    // layout
    centralWidget()->setLayout(layout);
}

CallContainer::~CallContainer()
{
    //delete proxy widgets
    delete dummyWidget;
    dummyWidget = NULL;
    delete fwdNumberWidget;
    fwdNumberWidget = NULL;
}

void CallContainer::toggleFwdNumberWidget(bool toggle)
{
    if(toggle) {
        lp->removeItem(dummyWidget);
        lp->addItemAtPosition(fwdNumberWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(dummyWidget);
        pp->addItemAtPosition(fwdNumberWidget, 3, 0, Qt::AlignCenter);
    } else {        
        lp->removeItem(fwdNumberWidget);
        lp->addItemAtPosition(dummyWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(fwdNumberWidget);
        pp->addItemAtPosition(dummyWidget, 3, 0, Qt::AlignCenter);
    }
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

void CallContainer::setCallForwarding(bool enabled, QString number)
{    
    qDebug() << Q_FUNC_INFO << enabled << number;

    if (number.compare(numberEdit->text()) != 0) {
        numberEdit->setText(number);
    }

    if (callFwdButton->isChecked() != enabled) {        
        callFwdButton->setChecked(enabled);
    } else {
        toggleFwdNumberWidget(enabled);
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
    case DcpCallAndSim::CallForwardingData:
        toggleFwdNumberWidget(false);

        callFwdButton->blockSignals(true);
        callFwdButton->setChecked(false);
        callFwdButton->blockSignals(false);
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

void CallContainer::callForwardingToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    toggleFwdNumberWidget(checked);

    emit callForwardingChanged(checked, numberEdit->text());
}

void CallContainer::numberChanged()
{
    QString number(numberEdit->text());

    qDebug() << Q_FUNC_INFO << callFwdButton->isChecked() << number;

    emit callForwardingChanged(callFwdButton->isChecked(), number);
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
