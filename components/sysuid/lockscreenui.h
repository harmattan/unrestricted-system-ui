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
    void updateDateTime();

private:
    DuiLayout* createWidgets();

private:
    DuiLabel* timeLabel; // qtn_scrlock_current_time
    DuiLabel* dateLabel; // qtn_scrlock_current_daydate

    DuiImage* unreadEmailsImage;
    DuiImage* unreadMessagesImage;
    DuiImage* missedCallsImage;
    DuiImage* unreadChatMessagesImage;

    DuiLabel* unreadEmailsLabel;
    DuiLabel* unreadMessagesLabel;
    DuiLabel* missedCallsLabel;
    DuiLabel* unreadChatMessagesLabel;

    UnlockSlider *slider;
};

#endif // LOCKSCREENUI_H
