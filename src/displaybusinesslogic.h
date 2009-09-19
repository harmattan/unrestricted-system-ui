#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H

#include "systemuigconf.h"
#include <qmsystem/qmdisplaystate.h>

#include <QObject>

using namespace Maemo;

class DisplayBusinessLogic : public QObject
{
public:
    DisplayBusinessLogic(SystemUIGConf *systemUIGConf);
    virtual ~DisplayBusinessLogic();

    void setBrightnessValue(const QString &value);
    void setScreenLightsValue(const QString &value);
    void setScreenLightsToggleValue(bool value);
    const QStringList& brightnessValues();
    const QStringList& screenLightsValues();
    const QVariant& GConfItemValue(SystemUIGConf::GConfKey key);

private: //methods
    void initSystemUIGConfKeys();

private: //attributes
    QmDisplayState *displayState;
    SystemUIGConf *systemUIGConf;

};

#endif // DISPLAYBUSINESSLOGIC_H
