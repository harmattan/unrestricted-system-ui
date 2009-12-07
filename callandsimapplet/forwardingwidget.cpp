#include "forwardingwidget.h"

//#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiButton>

#include <QGraphicsLinearLayout>
#include <QDebug>

ForwardingWidget::ForwardingWidget(const QString& label, const QString& toggleFunction, QGraphicsItem* parent) :
        QGraphicsWidget(parent),
        dbusFunction(toggleFunction)
{
    qDebug() << Q_FUNC_INFO << label;

    DuiLayout* layout = new DuiLayout();
    lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred
    pp = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(pp); // ownership transferred

    // checkbox widget (label and button)

    DuiLabel* checkLabel = new DuiLabel(label);
    checkLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    checkLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    checkButton = new DuiButton;
    checkButton->setObjectName("checkBoxButton");
    checkButton->setCheckable(true);

    QGraphicsWidget* checkWidget = new QGraphicsWidget;
    checkWidget->setPreferredWidth(1);
    QGraphicsLinearLayout* checkLayout = new QGraphicsLinearLayout(Qt::Horizontal, checkWidget);
    checkLayout->setContentsMargins(0, 0, 0, 0);

    checkLayout->addItem(checkLabel);
    checkLayout->setAlignment(checkLabel, Qt::AlignLeft | Qt::AlignVCenter);
    checkLayout->addItem(checkButton);
    checkLayout->setAlignment(checkButton, Qt::AlignRight | Qt::AlignVCenter);

    // number button

    numberButton = new DuiButton;
    numberButton->setObjectName("forwardingButton");

    // dummy placeholder for hiding the button

    dummyWidget = new QGraphicsWidget;
    dummyWidget->setContentsMargins(0, 0, 0, 0);
    dummyWidget->setPreferredWidth(1);

    // policies

    // adds dummyWidget to the policies so the button is hidden by default
    setButtonVisible(checkButton->isChecked());

    lp->setSpacing(5);
    lp->addItemAtPosition(checkWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // portrait policy

    pp->setSpacing(5);
    pp->addItemAtPosition(checkWidget, 0, 0, Qt::AlignCenter);

    // connect signals

    connect(checkButton, SIGNAL(toggled(bool)), this, SLOT(checked(bool)));
    connect(numberButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));

    setLayout(layout);
}

ForwardingWidget::~ForwardingWidget()
{
    if (checkButton->isChecked()) {
        delete dummyWidget;
    } else {
        delete numberButton;
    }
}

void ForwardingWidget::setButtonVisible(bool visible)
{
    qDebug() << Q_FUNC_INFO << visible;
    if (visible) {
        lp->removeItem(dummyWidget);
        lp->addItemAtPosition(numberButton, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(dummyWidget);
        pp->addItemAtPosition(numberButton, 1, 0, Qt::AlignCenter);
    } else {
        lp->removeItem(numberButton);
        lp->addItemAtPosition(dummyWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(numberButton);
        pp->addItemAtPosition(dummyWidget, 1, 0, Qt::AlignCenter);
    }
}

void ForwardingWidget::checked(bool check)
{
    qDebug() << Q_FUNC_INFO;
    setButtonVisible(check);

    if (!check) {
        emit forwardingDisabled(dbusFunction);
    }
}
