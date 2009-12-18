#ifndef NETWORKCELL_H
#define NETWORKCELL_H

#include <QObject>

namespace Cellular
{

class NetworkCell : public QObject
{
    Q_OBJECT

public:
    NetworkCell(QObject *parent = 0);
    ~NetworkCell();

    enum Service {
        CSService = 1,         //!< Current cell supports CS services
        GPRSService = 1 << 1,  //!< Current cell supports GPRS services
        GPRSSupport = 1 << 2,  //!< Current cell supports GPRS
        EGPRSSupport = 1 << 3, //!< Current cell supports EGPRS
        DTMSupport = 1 << 4,   //!< Current cell supports DTM
        HSDPASupport = 1 << 5, //!< Current cell supports HSDPA
        HSUPASupport = 1 << 6  //!< Current cell supports HSUPA
    };
    Q_DECLARE_FLAGS(Services, Service);

    //! Get the currently supported services in cell.
    NetworkCell::Services services() const;

    void setServices(NetworkCell::Services services);

signals:
    void servicesChanged(NetworkCell::Services);

private:
    NetworkCell::Services currentServices;
};
}

#endif  /* !NETWORKCELL_H */
