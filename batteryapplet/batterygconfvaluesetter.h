#ifndef BATTERYGCONFVALUESETTER_H
#define BATTERYGCONFVALUESETTER_H

#include <QThread>
#include <QVariant>

class DuiGConfItem;


class BatteryGConfValueSetter : public QThread
{
public:
    BatteryGConfValueSetter(DuiGConfItem *item, QVariant value);
    virtual ~BatteryGConfValueSetter();

    void run();

private:
    DuiGConfItem *item;
    QVariant value;
};

#endif // BATTERYGCONFVALUESETTER_H
