#ifndef NETWORKAPPLET_H
#define NETWORKAPPLET_H

#include "dcpappletif.h"
#include <QObject>
class DcpWidget;
class DuiAction;

class NetworkApplet : public QObject, public DcpAppletIf
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
#endif // NETWORKAPPLET_H
