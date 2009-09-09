#ifndef UNLOCKSLIDER_H
#define UNLOCKSLIDER_H

#include <DuiSlider>
#include <QBasicTimer>

class UnlockSlider : public DuiSlider
{
    Q_OBJECT

public:
    UnlockSlider(DuiWidget *parent, const QString &viewType);
    virtual ~UnlockSlider();

public slots:
    void reset();
    void pressed();
    void released();
    void moved(int val);

signals:
    void unlocked();

protected:
    void timerEvent(QTimerEvent *event);

private:
    QBasicTimer timer;
    float resetVelocity;
};

#endif // UNLOCKSLIDER_H
