#include "shutdowndialog.h"

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiStylableWidget>

ShutdownDialog::ShutdownDialog(const QString &title, StandardButton buttons) : DuiDialog(title, buttons)
{
    DuiLayout *centralLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *centralLayoutPolicy = new DuiLinearLayoutPolicy(centralLayout, Qt::Vertical);
    centralLayoutPolicy->addItemAtPosition(new DuiLabel(QString("BYE")), 0);    
    DuiStylableWidget *centralLayoutWidget = new DuiStylableWidget();
    centralLayoutWidget->setObjectName("layoutWidget");
    centralLayoutWidget->setLayout(centralLayout);

    this->setCentralWidget(centralLayoutWidget);
}

ShutdownDialog::~ShutdownDialog()
{
}
