#include "forwardingwidget.h"

#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiButton>
#include <DuiLocale>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
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
    numberButton->setText(trid("qtn_cell_enter_select_number", "Enter or select number"));

    // layout for button

    buttonWidget = new QGraphicsWidget;
    buttonWidget->setPreferredWidth(1);
    QGraphicsGridLayout* buttonLayout = new QGraphicsGridLayout(buttonWidget);
    checkLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addItem(numberButton, 0, 0, 1, 1, Qt::AlignCenter);

    // dummy placeholder for hiding the button

    dummyWidget = new QGraphicsWidget;
    dummyWidget->setContentsMargins(0, 0, 0, 0);
    dummyWidget->setPreferredWidth(1);

    // policies

    lp->setSpacing(5);
    lp->addItemAtPosition(checkWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // portrait policy

    pp->setSpacing(5);
    pp->addItemAtPosition(checkWidget, 0, 0, Qt::AlignCenter);

    // adds dummyWidget to the policies so the button is hidden by default
    showButton(checkButton->isChecked());

    // connect signals

    connect(checkButton, SIGNAL(toggled(bool)), this, SLOT(checked(bool)));
    connect(numberButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));

    setLayout(layout);
}

ForwardingWidget::~ForwardingWidget()
{
    // delete the one that's not visible because it doesn't have parent
    if (checkButton->isChecked()) {
        delete dummyWidget;
    } else {
        delete buttonWidget;
    }
}

void ForwardingWidget::update(bool enabled, QString number)
{
    showButton(enabled);
    numberButton->setText( number.length() > 0 ? number : trid("qtn_cell_enter_select_number", "Enter or select number"));
}

void ForwardingWidget::showButton(bool show)
{
    qDebug() << Q_FUNC_INFO << show;
    if (show) {
        lp->removeItem(dummyWidget);
        lp->addItemAtPosition(buttonWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(dummyWidget);
        pp->addItemAtPosition(buttonWidget, 1, 0, Qt::AlignCenter);
    } else {
        lp->removeItem(buttonWidget);
        lp->addItemAtPosition(dummyWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(buttonWidget);
        pp->addItemAtPosition(dummyWidget, 1, 0, Qt::AlignCenter);
    }
}

bool ForwardingWidget::isEnabled()
{
    return checkButton->isChecked();
}

void ForwardingWidget::checked(bool check)
{
    qDebug() << Q_FUNC_INFO;
    showButton(check);

    if (!check) {
        emit forwardingDisabled(dbusFunction);
    }
    emit enabled(check);
}
