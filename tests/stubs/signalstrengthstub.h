#ifndef SIGNALSTRENGTH_H
#define SIGNALSTRENGTH_H

#include <SignalStrength>
#include <QObject>
#include <QTimer>

namespace Cellular {

class SignalStrength : public QObject
{
    Q_OBJECT

public:
    SignalStrength(QObject *parent=0);
    ~SignalStrength();
    int bars() const;
    void setBars(int bars);

signals:
    void levelChanged(int bars, int dummy);

private:
    QTimer *timer;
    int currentBars;
    int dummy;
};

}

#endif  /* SIGNALSTRENGTH */
