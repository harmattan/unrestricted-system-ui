#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H

#include "systemuigconf.h"

#include <QObject>
#include <QStringList>

class DisplayBusinessLogic : public QObject
{
    Q_OBJECT

public:
    DisplayBusinessLogic();
    virtual ~DisplayBusinessLogic();

    void queryBrightnessValues();
    void queryScreenLightsValues();
    bool blankInhibitValue();
    void setBrightnessValue(const QString &value);
    void setScreenLightsValue(const QString &value);
    void setBlankInhibitValue(bool value);

signals:
    void brightnessValuesAvailable(int, QStringList);
    void screenLightsValuesAvailable(int, QStringList);

};

#endif // DISPLAYBUSINESSLOGIC_H
