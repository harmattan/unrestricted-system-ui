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

CallContainer::CallContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::CallText, parent),
        sendCallerIdComboBox(NULL),
        callWaitingButton(NULL),
        callFwdButton(NULL),
        numberEdit(NULL),
        pickerButton(NULL)
{
    setLayout();
}

void CallContainer::setLayout()
{
    // layout & policies

    DuiLayout* layout = new DuiLayout();

    DuiGridLayoutPolicy* lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred

    DuiGridLayoutPolicy* pp = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(pp); // ownership transferred

    // send caller id

    sendCallerIdComboBox = new DuiComboBox();
    // TODO: init combo box
    sendCallerIdComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QGraphicsLinearLayout* sendCallerIdLayout = new QGraphicsLinearLayout(Qt::Vertical);
    // TODO: some const value here some day
    sendCallerIdLayout->setContentsMargins(10, 10, 10, 10);
    sendCallerIdLayout->addItem(createLabel(DcpCallAndSim::SendCallerIdText));
    sendCallerIdLayout->addItem(sendCallerIdComboBox);

    // call waiting checkbox

    QGraphicsLinearLayout* callWaitingLayout =
            createCheckBox(DcpCallAndSim::CallWaitingText, callWaitingButton, QString("callWaitingButton"));

    // call forwarding checkbox

    QGraphicsLinearLayout* callFwdLayout =
            createCheckBox(DcpCallAndSim::CallForwardingText, callFwdButton, QString("callForwardingButton"));

    // call forwarding number

    numberEdit = new DuiTextEdit();
    // TODO: init textedit

    pickerButton = new DuiButton();
    pickerButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pickerButton->setObjectName("peoplePickerButton");

    QGraphicsGridLayout* fwdNumberLayout = new QGraphicsGridLayout();
    fwdNumberLayout->addItem(createLabel(DcpCallAndSim::ForwardToText), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    fwdNumberLayout->addItem(pickerButton, 0, 1, 2, 1, Qt::AlignCenter);
    fwdNumberLayout->addItem(numberEdit, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // landscape policy

    lp->addItemAtPosition(sendCallerIdLayout, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callWaitingLayout, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(callFwdLayout, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(fwdNumberLayout, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    lp->setSpacing(5);

    // portrait policy

    lp->addItemAtPosition(sendCallerIdLayout, 0, 0, Qt::AlignCenter);
    lp->addItemAtPosition(callWaitingLayout, 1, 0, Qt::AlignCenter);
    lp->addItemAtPosition(callFwdLayout, 2, 0, Qt::AlignCenter);
    lp->addItemAtPosition(fwdNumberLayout, 3, 0, Qt::AlignCenter);
    pp->setSpacing(5);

    // layout

    centralWidget()->setLayout(layout);
}

DuiLabel* createLabel(const QString& text)
{
    DuiLabel* label = new DuiLabel(text);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return label;
}

QGraphicsLinearLayout* CallContainer::createCheckBox(const QString& text, DuiButton*& button, const QString& name)
{
    button = new DuiButton();
    button->setObjectName(name);
    button->setCheckable(true);

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);
    // TODO: some const value here some day
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addItem(createLabel(text));
    layout->addItem(button);

    return layout;
}
