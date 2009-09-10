#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <DuiApplicationPage>
#include <QBasicTimer>

class DuiLabel;
class DuiImage;
class DuiButton;
class DuiFreestyleLayoutPolicy;
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
signals:
    void unlocked();

public slots:
    void sliderUnlocked();
    void orientationChanged(const Dui::Orientation &orientation);

private:
    void timerEvent(QTimerEvent *event);
    void updateDateTime();

private:

    DuiLabel *timeLabel; // qtn_scrlock_current_time
    DuiLabel *dateLabel; // qtn_scrlock_current_daydate
/*
    DuiImage *missedCallsImage;
    DuiImage *unreadMessagesImage;
    DuiImage *unreadChatMessagesImage;
*/
    UnlockSlider *slider;
    DuiFreestyleLayoutPolicy *p_policy;
    DuiFreestyleLayoutPolicy *l_policy;

    QBasicTimer timer;
    bool timerAdjusting;
};

#endif // LOCKSCREENUI_H
