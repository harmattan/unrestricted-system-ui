/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <DuiApplicationPage>
#include <QBasicTimer>

class DuiLabel;
class DuiImageWidget;
class DuiButton;
class DuiLayout;
class UnlockSlider;

class LockScreenUI : public DuiApplicationPage
{
    Q_OBJECT

public:
    LockScreenUI();
    virtual ~LockScreenUI();

    virtual void createContent ();
    void updateMissedEventAmounts (
            int emails, 
            int messages, 
            int calls, 
            int im);

signals:
    void unlocked ();

public slots:
    void sliderUnlocked ();
    void updateDateTime ();

private:
    DuiLayout* createWidgets ();

private:
    DuiLabel *timeLabel; // qtn_scrlock_current_time
    DuiLabel *dateLabel; // qtn_scrlock_current_daydate

    DuiImageWidget *unreadEmailsImage;
    DuiImageWidget *unreadMessagesImage;
    DuiImageWidget *missedCallsImage;
    DuiImageWidget *unreadChatMessagesImage;

    DuiLabel *unreadEmailsLabel;
    DuiLabel *unreadMessagesLabel;
    DuiLabel *missedCallsLabel;
    DuiLabel *unreadChatMessagesLabel;

    UnlockSlider *slider;
};

#endif // LOCKSCREENUI_H
