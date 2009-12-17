#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

#include <QObject>

namespace Cellular
{

class SystemControl : public QObject
{
    Q_OBJECT

public:
    SystemControl(QObject *parent=0);
    ~SystemControl();

    bool power() const;
    void setPower(bool value);

signals:
    void powerChanged(bool value);

};

}
#endif // SYSTEMCONTROL_H
