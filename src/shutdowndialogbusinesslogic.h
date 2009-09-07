#ifndef SHUTDOWNDIALOGBUSINESSLOGIC_H
#define SHUTDOWNDIALOGBUSINESSLOGIC_H

#include <QObject>

#include "shutdowndialog.h"

class ShutdownDialogBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownDialogBusinessLogic();
    virtual ~ShutdownDialogBusinessLogic();

private:
    ShutdownDialog *shutdownDlg;


public slots:
    void openDialog(bool buttonDown);


};

#endif // SHUTDOWNDIALOGBUSINESSLOGIC_H
