#ifndef NETWORKOPERATOR_H
#define NETWORKOPERATOR_H

#include <QObject>

namespace Cellular
{

class SSC: public QObject
{
    Q_OBJECT
public:
    //! Possible SSC errors
    enum SSCError { NoError, /*!< Returned when no error happened and operation completed successfully */
        BusCommunicationError, /*!< Returned when a D-Bus communication error with the SSC daemon occured */
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
};

class NetworkOperator : public QObject
{
    Q_OBJECT

public:
    NetworkOperator(QObject *parent=0);
    ~NetworkOperator();

    QString name() const;
    QString mnc() const;
    QString mcc() const;

    void setName(const QString &name);
    void setMnc(const QString &mnc);
    void setMcc(const QString &mcc);

    bool isValid();

signals:
    void nameChanged(const QString &name);

private:
    QString currentName;
    QString currentMnc;
    QString currentMcc;

};

}

#endif  /* !NETWORKOPERATOR_H */
