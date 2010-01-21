#ifndef BATTERYAPPLET_H
#define BATTERYAPPLET_H

#include <DcpAppletIf>
#include <QObject>

class DcpWidget;
class DuiAction;
class BatteryWidget;

class BatteryApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    BatteryApplet ();
    ~BatteryApplet ();
    
    virtual void init();
    virtual DcpWidget *constructWidget(int widgetId);
    virtual DcpWidget *pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId = 0);

private:
    BatteryWidget  *m_MainWidget;

};
#endif // BATTERYAPPLET_H
