#ifndef PROFILEAPPLET_H
#define PROFILEAPPLET_H

#include "dcpappletif.h"
#include <QObject>
class ProfileWidget;
class DuiAction;

class ProfileApplet : public QObject, public DcpAppletIf
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

private slots:
    void doRotation();
    void startRotation();

private:
    QPointer<ProfileWidget> main;

};
#endif // PROFILEAPPLET_H
