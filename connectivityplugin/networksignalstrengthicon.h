#ifndef NETWORKSIGNALSTRENGTHICON_H
#define NETWORKSIGNALSTRENGTHICON_H

#include <DuiImage>
#include <SignalStrength>

using namespace Cellular;

class NetworkSignalStrengthIcon : public DuiImage
{
    Q_OBJECT

public:
    NetworkSignalStrengthIcon(QGraphicsItem *parent = 0);
    virtual ~NetworkSignalStrengthIcon();   

private slots:
    void signalStrengthChanged(int bars);

private:
    QString mapSignalStrengthToIcon(int bars);

private:
    SignalStrength *signalStrength;

};

#endif // NETWORKSIGNALSTRENGTHICON_H
