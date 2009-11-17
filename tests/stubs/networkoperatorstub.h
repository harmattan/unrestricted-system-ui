#ifndef NETWORKOPERATOR_H
#define NETWORKOPERATOR_H

#include <QObject>

namespace Cellular
{

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

signals:
    void nameChanged(const QString &name);

private:
    QString currentName;
    QString currentMnc;
    QString currentMcc;

};

}

#endif  /* !NETWORKOPERATOR_H */
