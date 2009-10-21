#ifndef DISPLAYAPPLET_H
#define DISPLAYAPPLET_H

#include "dcpappletif.h"
#include <QObject>
class DcpWidget;
class DuiAction;
class DisplayWidget;

class DisplayApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<DisplayWidget> main;

};
#endif // DISPLAYAPPLET_H
