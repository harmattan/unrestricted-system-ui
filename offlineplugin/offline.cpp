#include "offline.h"

#include <DuiStatusIndicatorMenuPluginInterface>


Offline::Offline(DuiStatusIndicatorMenuInterface& statusIndicatorMenu, QGraphicsItem* parent) :
        DuiWidget(parent),
        statusIndicatorMenu(statusIndicatorMenu)
{
}

Offline::~Offline()
{
}
