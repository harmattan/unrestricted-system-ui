#include "shutdowndialog.h"

#include <QDebug>

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiProgressIndicator>
#include <DuiStylableWidget>

ShutdownDialog::ShutdownDialog(const QString &title, StandardButton buttons, int sliderRange) : DuiDialog(title, buttons),
    sliderRange(sliderRange)
{ 
    DuiLayout *centralLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *centralLayoutPolicy = new DuiLinearLayoutPolicy(centralLayout, Qt::Vertical);
    centralLayoutPolicy->addItemAtPosition(new DuiLabel(QString("BYE")), 0);    

    aSlider = new UnlockSlider(this, "continuous", sliderRange);
    aSlider->setOrientation(Qt::Horizontal);
    aSlider->setMaximumHeight(70);
    aSlider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    aSlider->setVisible(true);
    centralLayoutPolicy->addItemAtPosition(aSlider, 1);

    DuiStylableWidget *centralLayoutWidget = new DuiStylableWidget();
    centralLayoutWidget->setObjectName("layoutWidget");
    centralLayoutWidget->setLayout(centralLayout);

    this->setCentralWidget(centralLayoutWidget); 
}

ShutdownDialog::~ShutdownDialog()
{
}

UnlockSlider* ShutdownDialog::slider()
{
    return aSlider;
}
