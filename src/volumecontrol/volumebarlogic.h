#ifndef VOLUMEBARLOGIC_H
#define VOLUMEBARLOGIC_H

#include <QObject>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>

class VolumeBarLogic : public QObject
{
    Q_OBJECT

    public:
        VolumeBarLogic ();
        ~VolumeBarLogic ();

        quint32 getVolume ();
        quint32 getMaxVolume ();

    public slots:
        void setVolume (quint32 value);
        void stepsUpdated (quint32 value, quint32 maxvalue);

    signals:
        void volumeChanged (quint32 value, quint32 maxvalue, bool init);

    private slots:
        void initValues ();

    private:
        void addSignalMatch ();

        DBusConnection  *m_dbus_conn;

        quint32     m_currentvolume;
        quint32     m_currentmax;

#ifdef UNIT_TEST
    friend class Ut_VolumeBarLogic;
#endif
};

#endif

