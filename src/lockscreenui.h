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
/*
    bool screenLock();
    void toggleScreenLock(bool toggle);
    bool sleepMode();
    void toggleSleepMode(bool toggle);
*/
    void updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages);

signals:
    void unlocked();

public slots:
    void sliderUnlocked();
    void orientationChanged(const Dui::Orientation &orientation);

private:
    void timerEvent(QTimerEvent *event);
    void updateDateTime();
    void calculateRects(DuiLayout* layout, QRect& timeRect, QRect& dateRect,
                        QRect& a, QRect& b, QRect& c,
                        QRect& a2, QRect& b2, QRect& c2,
                        QRect& sliderRect);

private:

    DuiLabel *timeLabel; // qtn_scrlock_current_time
    DuiLabel *dateLabel; // qtn_scrlock_current_daydate

    DuiImage
        *missedCallsImage,
        *unreadMessagesImage,
        *unreadChatMessagesImage;

    DuiLabel
        *missedCallsLbl,
        *unreadMessagesLbl,
        *unreadChatMessagesLbl;

    UnlockSlider *slider;

    QBasicTimer timer;
    bool timerAdjusting;
};

#endif // LOCKSCREENUI_H
