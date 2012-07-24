#include "safemodeplugin.h"
#include "safemodecontroller.h"

Q_EXPORT_PLUGIN2(safemodeplugin, SafeModePlugin)

SafeModePlugin::SafeModePlugin(QObject *parent) :
    statusIndicatorMenu(0),
    m_safeModeController(0)
{
    Q_UNUSED(parent)
}

void SafeModePlugin::setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface)
{
    statusIndicatorMenu = &menuInterface;
}

MStatusIndicatorMenuInterface *SafeModePlugin::statusIndicatorMenuInterface() const
{
    return statusIndicatorMenu;
}

// Methods derived from MApplicationExtensionInterface
bool SafeModePlugin::initialize(const QString &)
{
    m_safeModeController = new SafeModeController(this);

    return true;
}

QGraphicsWidget *SafeModePlugin::widget()
{
    return m_safeModeController;
}

M_LIBRARY
