#include <networkcellstub.h>

namespace Cellular
{

NetworkCell::NetworkCell(QObject *parent) :
        QObject(parent)
{
}

NetworkCell::~NetworkCell()
{
}

NetworkCell::Services NetworkCell::services() const
{
    return currentServices;
}

void NetworkCell::setServices(NetworkCell::Services services)
{
    currentServices = services;
    emit servicesChanged(currentServices);
}

}
