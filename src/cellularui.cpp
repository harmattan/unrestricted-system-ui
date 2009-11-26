#include "cellularui.h"

#include <DuiLocale>

#include <QDBusInterface>
#include <QDebug>

CellularUI::CellularUI(QObject* parent) :
        QObject(parent)
{
    // Note! This D-Bus interface is going to be replaced by some Qt bindings.
    gprs = new QDBusInterface("com.nokia.csd.GPRS",
                              "/com/nokia/csd/gprs",
                              "com.nokia.csd.GPRS",
                              QDBusConnection::systemBus(),
                              this);

    connect(gprs, SIGNAL(Suspended(int, QString)), this, SLOT(gprsSuspended(int, QString)));
}

void CellularUI::gprsSuspended(int code, QString cause)
{
    Q_UNUSED(code);
    Q_UNUSED(cause);

    /*
           uint32  string
           code    cause
           ----    -----
           0x02    Attached
           0x03    Detached
           0x04    Resumed
           0x05    No coverage
           0x07    Call or SMS signalling
           0x08    Circuit switched call active
           0x09    Routing area update
           0x0A    Location update
    */

    emit showNotification(trid("qtn_cell_network_data_unavailable", "Phone network data temporarily unavailable"));
}
