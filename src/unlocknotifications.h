#ifndef UNLOCKNOTIFICATIONS_H
#define UNLOCKNOTIFICATIONS_H

#include <QObject>
#include <MSceneWindow>

class QPixmap;
class MLabel;
class MImageWidget;
class QGraphicsLinearLayout;

class UnlockNotifications : public MSceneWindow
{
    Q_OBJECT

    enum {
        NOTIFY_CALLS = 0,
        NOTIFY_SMS,
        NOTIFY_EMAIL,
        NOTIFY_CHAT,
        NOTIFY_LAST
    };

public:
    UnlockNotifications ();
    virtual ~UnlockNotifications ();

public slots:
    void updateMissedEvents (int emails,
                             int messages,
                             int calls,
                             int im);

private:
    QPixmap                 *m_background;
    MLabel                  *m_labels [NOTIFY_LAST];
    MImageWidget            *m_icons [NOTIFY_LAST];
    QGraphicsLinearLayout   *m_layout;
};



#endif
