#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <DuiApplicationPage>
#include <QBasicTimer>

class DuiLabel;
class DuiImage;
class DuiButton;
class DuiLayout;
class UnlockSlider;

class LockScreenUI : public DuiApplicationPage
{
    Q_OBJECT

public:
    LockScreenUI();
    virtual ~LockScreenUI();
    virtual void createContent();

    void updateMissedEventAmounts(int a, int b, int c, int d);

signals:
    void unlocked();

public slots:
    void sliderUnlocked();
    void orientationChanged(const Dui::Orientation &orientation);
    void updateDateTime();

private:
    void timerEvent(QTimerEvent *event);
    void calculateRects(DuiLayout* layout, QRect& timeRect, QRect& dateRect,
                        QRect& a, QRect& b, QRect& c, QRect& d,
                        QRect& a2, QRect& b2, QRect& c2, QRect& d2,
                        QRect& sliderRect);

private:

    DuiLabel *timeLabel; // qtn_scrlock_current_time
    DuiLabel *dateLabel; // qtn_scrlock_current_daydate

    DuiImage
    *unreadEmailsImage,
    *unreadMessagesImage,
    *missedCallsImage,
    *unreadChatMessagesImage;

    DuiLabel
    *unreadEmailsLbl,
    *unreadMessagesLbl,
    *missedCallsLbl,
    *unreadChatMessagesLbl;

    UnlockSlider *slider;

    QBasicTimer timer;
    bool timerAdjusting;
};

#endif // LOCKSCREENUI_H
