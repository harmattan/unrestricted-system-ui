#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H

#include "systemuigconf.h"
//#include <qmsystem/qmdisplaystate.h> // stub used
#include "displaystatestub.h"

#include <QObject>
#include <QStringList>

using namespace Maemo;

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

private:
    QmDisplayState *display;

};

#endif // DISPLAYBUSINESSLOGIC_H
