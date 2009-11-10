#include "network.h"

#include <DuiStatusIndicatorMenuPluginInterface>

Network::Network(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
        DuiWidget(parent),
        statusIndicatorMenu(statusIndicatorMenu)
{
}

Network::~Network()
{
}
