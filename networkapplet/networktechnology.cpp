#include "networktechnology.h"
#include <QDebug>

const int NetworkTechnologyCheckInterval = 500;

NetworkTechnology::NetworkTechnology(RadioAccess *radioAccess, QObject *parent) :
        QObject(parent),
        radioAccess(radioAccess),
        networkCell(NULL),
        timer(NULL)
{
    qDebug() << Q_FUNC_INFO;
    networkCell = new NetworkCell(this);
    /* Technology changes depend to the RadioAccess's techonology and NetworkCell's services.
       The state changes of both of those have to be taken into account before we indicate any
       changes to end-user. There will be 500 ms interval which makes it possible that both the
       signals are catched before acting. */
    timer = new QTimer(this);
    timer->setInterval(NetworkTechnologyCheckInterval);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTechnology()));
    connect(radioAccess, SIGNAL(technologyChanged(int)), timer, SLOT(start()));
    connect(radioAccess, SIGNAL(statusChanged(RadioAccess::Flags)), timer, SLOT(start()));
    connect(networkCell, SIGNAL(servicesChanged(NetworkCell::Services)), timer, SLOT(start()));
    updateTechnology();
}

NetworkTechnology::~NetworkTechnology()
{
}

void NetworkTechnology::checkTechnology()
{
    qDebug() << Q_FUNC_INFO;
    timer->stop();
    NetworkTechnology::Technology previousTechnology = currentTechnology();
    updateTechnology();
    if (previousTechnology != currentTechnology())
        emit technologyChanged(technology);
}

NetworkTechnology::Technology NetworkTechnology::currentTechnology()
{
    qDebug() << Q_FUNC_INFO;
    return technology;
}

void NetworkTechnology::updateTechnology()
{
    qDebug() << Q_FUNC_INFO << " " << radioAccess->technology() << " " << networkCell->services();
    technology = NetworkTechnology::None;
    switch (radioAccess->technology()) {
    case RadioAccess::GSM:
        technology = NetworkTechnology::TwoG;
        if (networkCell->services() == NetworkCell::EGPRSSupport)
            technology = NetworkTechnology::TwoPointFiveG;
        break;
    case RadioAccess::UMTS:
        technology = NetworkTechnology::ThreeG;
        if (networkCell->services() == NetworkCell::HSDPASupport && radioAccess->state() == RadioAccess::AllocatedHSDPA)
            technology = NetworkTechnology::ThreePointFiveG;
        break;
    default:
        break;
    }
}
