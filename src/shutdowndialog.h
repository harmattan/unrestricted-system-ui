#ifndef SHUTDOWNDIALOG_H
#define SHUTDOWNDIALOG_H

#include <QObject>
#include <DuiDialog>

class ShutdownDialog : public DuiDialog
{
    Q_OBJECT

public:
    ShutdownDialog(const QString &title, StandardButton buttons = NoButton);
    virtual ~ShutdownDialog();
};

#endif // SHUTDOWNDIALOG_H
