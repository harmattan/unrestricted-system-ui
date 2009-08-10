#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <DuiApplicationPage>

class DuiLabel;
class DuiImage;
class DuiButton;

class LockScreenUI : public DuiApplicationPage
{
    Q_OBJECT

public:
    LockScreenUI();
    virtual ~LockScreenUI();

private:
    DuiLabel *timeLabel; // qtn_scrlock_current_time
    DuiLabel *dateLabel; // qtn_scrlock_current_daydate
    DuiImage *missedCallsImage;
    DuiImage *unreadMessagesImage;
    DuiImage *unreadChatMessagesImage;
    DuiButton *unlockSliderButton;
    DuiImage *unlockIconImage;
    DuiImage *deviceLockIconImage;
    DuiImage *defaultBackgroundImage;
};

#endif // LOCKSCREENUI_H
