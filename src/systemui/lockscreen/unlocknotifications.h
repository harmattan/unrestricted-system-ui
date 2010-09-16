#ifndef UNLOCKNOTIFICATIONS_H
#define UNLOCKNOTIFICATIONS_H

#include <QObject>
#include <QHash>
#include <MStylableWidget>
#include <MSceneWindow>

class MLabel;
class MImageWidget;
class QGraphicsLinearLayout;
class QShowEvent;

class UnlockNotifications : public MStylableWidget
{
    Q_OBJECT

public:
    UnlockNotifications ();
    virtual ~UnlockNotifications ();

signals:
    void needToShow (bool visible);

private slots:
    void updateContents ();

protected:  
    virtual void orientationChangeEvent (MOrientationChangeEvent *event);

    virtual QSizeF sizeHint (Qt::SizeHint which,
                             const QSizeF& constraint = QSizeF()) const;
    
    void orientationChanged (M::Orientation orientation);

private:
    /*
     * for other events area:
     */
    QHash<int, MLabel *>         m_labels;
    QHash<int, MImageWidget *>   m_icons;
    QHash<int, QString>          m_icon_ids;
    QGraphicsLinearLayout       *m_icon_layout;

    MStylableWidget             *m_otherevents_area;
    /*
     * for most recent event area:
     */
    MLabel                      *m_last_subject;
    MImageWidget                *m_last_icon;
    QGraphicsLinearLayout       *m_mostrecent_layout;

    MStylableWidget             *m_mostrecent_area;
    /*
     * main vbox
     */
    QGraphicsLinearLayout       *m_vbox;
    #ifdef UNIT_TEST
    friend class Ut_UnlockNotifications;
    #endif
};

#endif
