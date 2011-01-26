#ifndef _QMUSBMODE_STUB_H
#define _QMUSBMODE_STUB_H

#include <QObject>

MeeGo::QmUSBMode::Mode testMode = MeeGo::QmUSBMode::Undefined;
MeeGo::QmUSBMode::Mode testDefaultMode = MeeGo::QmUSBMode::Undefined;

MeeGo::QmUSBMode::QmUSBMode(QObject *parent)
{
    Q_UNUSED (parent);
}

MeeGo::QmUSBMode::~QmUSBMode()
{
}

MeeGo::QmUSBMode::Mode MeeGo::QmUSBMode::getMode()
{
    return testMode;
}

bool MeeGo::QmUSBMode::setMode(MeeGo::QmUSBMode::Mode mode)
{
    testMode = mode;
    return true;
}

bool MeeGo::QmUSBMode::setDefaultMode(MeeGo::QmUSBMode::Mode mode)
{
    testDefaultMode = mode;
    return true;
}

MeeGo::QmUSBMode::Mode MeeGo::QmUSBMode::getDefaultMode()
{
    return testDefaultMode;
}

void MeeGo::QmUSBMode::connectNotify(const char *)
{
}
void MeeGo::QmUSBMode::disconnectNotify(const char *)
{
}
#endif
