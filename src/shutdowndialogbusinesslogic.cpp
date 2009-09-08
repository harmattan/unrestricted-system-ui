#include <QDebug>

#include "shutdowndialogbusinesslogic.h"


ShutdownDialogBusinessLogic::ShutdownDialogBusinessLogic()
{
    shutdownDlg = NULL;
}
ShutdownDialogBusinessLogic::~ShutdownDialogBusinessLogic()
{
    if (shutdownDlg)
        shutdownDlg->deleteLater();
    shutdownDlg = NULL;
}

void ShutdownDialogBusinessLogic::openDialog(bool btnDown)
{
    qDebug() << "open dlg";
    if (shutdownDlg != NULL)
        return;

    shutdownDlg = new ShutdownDialog(QString("Shutdown dialog"), DuiDialog::NoButton);

    qDebug() << "exec";
    shutdownDlg->exec();

}
