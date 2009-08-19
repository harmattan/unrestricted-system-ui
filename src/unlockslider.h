#ifndef UNLOCKSLIDER_H
#define UNLOCKSLIDER_H

#include <DuiSlider>

class UnlockSlider : public DuiSlider
{
    Q_OBJECT

public:
    UnlockSlider(DuiWidget *parent, const QString &viewType);
    virtual ~UnlockSlider();

public slots:
    void reset();
    void released();
    void moved(int val);

signals:
    void unlocked();
};

#endif // UNLOCKSLIDER_H
