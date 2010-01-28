#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H
#include <qmsystem/qmdisplaystate.h>

#include <QObject>

using namespace Maemo;

class DisplayBusinessLogic : public QObject
{
    Q_OBJECT

public:
    DisplayBusinessLogic(QObject* parent = 0);
    virtual ~DisplayBusinessLogic();

    QList<int> brightnessValues();
    int selectedBrightnessValue();
    QList<int> screenLightsValues();
    int selectedScreenLightsValue();
    bool blankInhibitValue();

public slots:
    void setBrightnessValue(int value);
    void setScreenLightTimeouts (int index);
    void setBlankInhibitValue(bool value);
    void toggleDisplay(bool toggle);

private: //attributes
    QmDisplayState *display;

};

#endif // DISPLAYBUSINESSLOGIC_H
