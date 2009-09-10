#ifndef SHUTDOWNDIALOG_H
#define SHUTDOWNDIALOG_H

#include <QObject>
#include <DuiDialog>

#include "unlockslider.h"

class ShutdownDialog : public DuiDialog
{
    Q_OBJECT

public:
    ShutdownDialog(const QString &title, StandardButton buttons = NoButton, int sliderRange = 100);
    virtual ~ShutdownDialog();

    UnlockSlider* slider();

private:
    UnlockSlider *aSlider;
    int sliderRange;

};

#endif // SHUTDOWNDIALOG_H
