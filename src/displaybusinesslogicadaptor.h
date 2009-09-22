#ifndef DISPLAYBUSINESSLOGICADAPTOR_H
#define DISPLAYBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class DisplayBusinessLogic;

class DisplayBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.display")

public:
    DisplayBusinessLogicAdaptor(QObject *obj, DisplayBusinessLogic *displayLogic);
    virtual ~DisplayBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void setBrightnessValue(const QString &value);
    Q_NOREPLY void setBlankTimeoutValue(const QString &value);
    Q_NOREPLY void setDimTimeoutValue(const QString &value);
    Q_NOREPLY void setBlankInhibitValue(bool value);
    QString brightnessValue();
    QString blankTimeoutValue();
    QString dimTimeoutValue();
    bool blankInhibitValue();    
    QStringList brightnessValues();
    QStringList blankTimeoutValues();
    QStringList dimTimeoutValues();

private: //attributes
    DisplayBusinessLogic *displayLogic;
};

#endif // DISPLAYBUSINESSLOGICADAPTOR_H
