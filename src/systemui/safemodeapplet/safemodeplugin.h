#ifndef SAFEMODEPLUGIN_H
#define SAFEMODEPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <MLibrary>
#include <MStatusIndicatorMenuExtensionInterface>
#include <MApplicationExtensionInterface>
#include <QGraphicsWidget>

class SafeModeController;

class SafeModePlugin : public QObject, public MStatusIndicatorMenuExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuExtensionInterface MApplicationExtensionInterface)

public:
    explicit SafeModePlugin(QObject *parent = 0);

    // Getter for the status indicator menu interface
    MStatusIndicatorMenuInterface *statusIndicatorMenuInterface() const;

    // Methods derived from MStatusIndicatorMenuPlugin
    virtual void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface);

    // Methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

signals:

public slots:

private:
    MStatusIndicatorMenuInterface *statusIndicatorMenu;
    SafeModeController *m_safeModeController;
};

#endif // SAFEMODEPLUGIN_H
