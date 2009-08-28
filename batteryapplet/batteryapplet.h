#ifndef BATTERYAPPLET_H
#define BATTERYAPPLET_H

#include "dcpappletif.h"
#include <QObject>
class DcpWidget;
class DuiAction;

class BatteryApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    virtual void init(QString part="");
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief();

};
#endif // BATTERYAPPLET_H
