#ifndef SSC_H
#define SSC_H

#include <QObject>

namespace Cellular
{

class SSC : public QObject
{
    Q_OBJECT

public:

    //! SSC errors
    enum SSCError
    {
        NoError = 0, /*!< Returned when no error happened and operation completed
                       successfully */
        BusCommunicationError, /*!< Returned when a D-Bus communication error with
                                 the SSC daemon occured */
        Failed,
        NotAvailable,
        ServiceNeeded,
        Internal,
        NotReady,
        MissingSim,
        UnsupportedSim,
        BlockedSim,
        SimLocked,
        MissingPin,
        SystemNotReady,
        Inactive,
        SearchingNetwork,
        NoCoverage,
        SelectedNetNotAvailable,
        Offline,
        ShuttingDown,
        PowerOff
    };

    SSC(QObject *parent=0);
    ~SSC();

public slots:
    void setRadioOn();
    void setRadioOff();

signals:
    void setRadioComplete(SSC::SSCError error);
};

}
#endif // SSC_H
