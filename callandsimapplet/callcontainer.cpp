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
        pickerButton(NULL)
{
    // layout & policies

    DuiLayout* layout = new DuiLayout();

    DuiGridLayoutPolicy* lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred

    DuiGridLayoutPolicy* pp = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(pp); // ownership transferred

    // send caller id

    sendCallerIdComboBox = new DuiComboBox();
//    sendCallerIdComboBox->setObjectName("sendCallerIdComboBox");
    sendCallerIdComboBox->addItem(DcpCallAndSim::LetNetworkChooseText);
    sendCallerIdComboBox->addItem(DcpCallAndSim::YesText);
    sendCallerIdComboBox->addItem(DcpCallAndSim::NoText);
    sendCallerIdComboBox->setIconVisible(false);
    sendCallerIdComboBox->setCurrentIndex(0);
//    sendCallerIdComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QGraphicsLinearLayout* sendCallerIdLayout = new QGraphicsLinearLayout(Qt::Vertical);
    // TODO: some const value here some day
    sendCallerIdLayout->setContentsMargins(10, 10, 10, 10);
    sendCallerIdLayout->addItem(createLabel(DcpCallAndSim::SendCallerIdText));
    sendCallerIdLayout->addItem(sendCallerIdComboBox);

    // call waiting checkbox

    QGraphicsLinearLayout* callWaitingLayout = createCheckBox(DcpCallAndSim::CallWaitingText, callWaitingButton);

    // call forwarding checkbox

    QGraphicsLinearLayout* callFwdLayout = createCheckBox(DcpCallAndSim::CallForwardingText, callFwdButton);

    // call forwarding number

    numberLabel = createLabel(DcpCallAndSim::ForwardToText);

    numberEdit = new DuiTextEdit();

    pickerButton = new DuiButton();
//    pickerButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pickerButton->setObjectName("peoplePickerButton");

    QGraphicsGridLayout* fwdNumberLayout = new QGraphicsGridLayout();
    fwdNumberLayout->addItem(numberLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    fwdNumberLayout->addItem(pickerButton, 0, 1, 2, 1, Qt::AlignCenter);
    fwdNumberLayout->addItem(numberEdit, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // landscape policy

    lp->setSpacing(5);
    lp->setColumnMaximumWidth(0, 390);
    lp->setColumnMaximumWidth(1, 390);
    lp->addItemAtPosition(sendCallerIdLayout, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callWaitingLayout, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callFwdLayout, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(fwdNumberLayout, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // portrait policy

    pp->setSpacing(5);
    pp->setColumnMaximumWidth(0, 470);
    pp->addItemAtPosition(sendCallerIdLayout, 0, 0, Qt::AlignCenter);
    pp->addItemAtPosition(callWaitingLayout, 1, 0, Qt::AlignCenter);
    pp->addItemAtPosition(callFwdLayout, 2, 0, Qt::AlignCenter);
    pp->addItemAtPosition(fwdNumberLayout, 3, 0, Qt::AlignCenter);

    // connect signals

    connect(sendCallerIdComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendCallerIdSelected(int)));
    connect(callWaitingButton, SIGNAL(toggled(bool)), this, SLOT(callWaitingToggled(bool)));
    connect(callFwdButton, SIGNAL(toggled(bool)), this, SLOT(callForwardingToggled(bool)));
    connect(numberEdit, SIGNAL(lostFocus(Qt::FocusReason)), this, SLOT(numberChanged()));

    // layout

    centralWidget()->setLayout(layout);
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
        numberLabel->setVisible(enabled);
        numberEdit->setVisible(enabled);
        pickerButton->setVisible(enabled);
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
            numberLabel->setVisible(false);
            numberEdit->setVisible(false);
            pickerButton->setVisible(false);

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

    numberLabel->setVisible(checked);
    numberEdit->setVisible(checked);
    pickerButton->setVisible(checked);

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

QGraphicsLinearLayout* CallContainer::createCheckBox(const QString& text, DuiButton*& button)
{
    button = new DuiButton();
    button->setObjectName("checkBoxButton");
    button->setCheckable(true);

    DuiLabel* label = createLabel(text);

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);

    layout->addItem(label);
    layout->setAlignment(label, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addItem(button);
    layout->setAlignment(button, Qt::AlignRight | Qt::AlignVCenter);

    return layout;
}
