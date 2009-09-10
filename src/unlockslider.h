#ifndef UNLOCKSLIDER_H
#define UNLOCKSLIDER_H

#include <DuiSlider>
#include <QBasicTimer>

class UnlockSlider : public DuiSlider
{
    Q_OBJECT

public:
    UnlockSlider(DuiWidget *parent, const QString &viewType, int range = 100);
    virtual ~UnlockSlider();

    void hwKeyDown(bool down);
    void updateValue(int newValue);

public slots:
    void reset();
    void pressed();
    void released();
    void moved(int val);    

signals:
    void unlocked();
    void valueChanged();

protected:
    void timerEvent(QTimerEvent *event);

private:
    QBasicTimer timer;
    float resetVelocity;
    int range;
    bool aHWKeyDown;
};

#endif // UNLOCKSLIDER_H
