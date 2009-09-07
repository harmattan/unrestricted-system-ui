#ifndef SHUTDOWNDIALOG_H
#define SHUTDOWNDIALOG_H

#include <QObject>
#include <DuiDialog>

class ShutdownDialog : public DuiDialog
{
    Q_OBJECT

public:
    ShutdownDialog();
    virtual ~ShutdownDialog();
};

#endif // SHUTDOWNDIALOG_H
