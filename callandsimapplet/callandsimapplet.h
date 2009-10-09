#ifndef CALLANDSIMAPPLET_H
#define CALLANDSIMAPPLET_H

#include "dcpappletif.h"
#include <QObject>

class DuiAction;
class CallAndSimWidget;

class CallAndSimApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    CallAndSimApplet();

    virtual void init(QString part = "");
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction*> viewMenuItems();
    virtual DcpBrief* constructBrief();

private:
    QPointer<CallAndSimWidget> main;
};

#endif // CALLANDSIMAPPLET_H
