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
    void setBlankTimeoutValue(const QString &value);
    void setDimTimeoutValue(const QString &value);
    void setBlankInhibitValue(bool value);    
    QStringList brightnessValues();
    QStringList blankTimeoutValues();
    QStringList dimTimeoutValues();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);


private: //methods
    QVariant defaultValue(SystemUIGConf::GConfKey key);


private: //attributes
    QmDisplayState *displayState;
    SystemUIGConf *systemUIGConf;

};

#endif // DISPLAYBUSINESSLOGIC_H
