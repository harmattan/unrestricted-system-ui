#ifndef BATTERYGCONFLISTENER_H
#define BATTERYGCONFLISTENER_H

#include <QObject>
#include <DuiConf>

class BatteryGConfListener : public QObject
{
    Q_OBJECT

public:
    BatteryGConfListener();
    virtual ~BatteryGConfListener();

public slots:
    void test(QString key, double val);
};

#endif // BATTERYGCONFLISTENER_H
