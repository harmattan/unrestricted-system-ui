#ifndef DISPLAYBUSINESSLOGICADAPTOR_H
#define DISPLAYBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class DisplayBusinessLogic;

class DisplayBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.DBus.DisplayBusinessLogic")

public:
    DisplayBusinessLogicAdaptor(DisplayBusinessLogic *displayLogic);
    virtual ~DisplayBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void setBrightnessValue(const QString &value);
    Q_NOREPLY void setScreenLightsValue(const QString &value);
    Q_NOREPLY void setScreenLightsToggleValue(bool value);
    QString brightnessValue();
    QString screenLightsValue();
    bool screenLightsToggleValue();
    QStringList brightnessValues();
    QStringList screenLightsValues();

private: //attributes
    DisplayBusinessLogic *displayLogic;
};

#endif // DISPLAYBUSINESSLOGICADAPTOR_H
