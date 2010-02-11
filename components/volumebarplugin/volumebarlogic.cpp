#include "volumebarlogic.h"

#include <QString>
#include <pulse/pulseaudio.h>
#include <pulse/thread-mainloop.h>

#define qstring2char(p) p->toLatin1().constData()

#undef DEBUG
#define WARNING
#include "../debug.h"

// This code based on maemo-statusmenu-volume code.
// TODO: FIXME: i need more information how this
//              should be done properly...

struct _NsvVolume {
    pa_threaded_mainloop    *mainloop;
    pa_context              *context;
    QString                 *default_sink;
    pa_volume_t              volume;
};

static void
context_state_cb (pa_context *c, void *userdata);

static void
server_info_cb (pa_context *c, const pa_server_info *i, void *userdata);

static void
sink_info_cb (pa_context *c, const pa_sink_info *i, int eol, void *userdata);


VolumeBarLogic::VolumeBarLogic () :
    m_priv (0),
    m_initialized (false)
{
    m_initialized = false;
    m_priv = new NsvVolume;

    m_priv->default_sink = 0;

    m_priv->mainloop = pa_threaded_mainloop_new ();

    if (! m_priv->mainloop)
    {
        SYS_WARNING ("Failed to create threaded mainloop");
        return;
    }

    if (pa_threaded_mainloop_start (m_priv->mainloop) < 0)
    {
        SYS_WARNING ("Failed to start the mainloop");
        return;
    }

    pa_threaded_mainloop_lock (m_priv->mainloop);

    m_priv->context =
        pa_context_new (pa_threaded_mainloop_get_api (m_priv->mainloop),
                        "nsv-volume");

    if (! m_priv->context)
    {
        SYS_WARNING ("Failed to create context: %s",
                     pa_strerror (pa_context_errno (m_priv->context)));
        pa_threaded_mainloop_unlock (m_priv->mainloop);
        return;
    }

    if (pa_context_connect (m_priv->context, 0, (pa_context_flags_t) 0, 0) < 0)
    {
        SYS_WARNING ("Failed to connect Pulseaudio: %s",
                     pa_strerror (pa_context_errno (m_priv->context)));
    }

    pa_context_set_state_callback (m_priv->context, context_state_cb, (void *) m_priv);

    /* Wait for the context to be ready */
    while (true)
    {
        pa_context_state_t state;

        state = pa_context_get_state (m_priv->context);

        SYS_DEBUG ("state = %d", state);

        if (! PA_CONTEXT_IS_GOOD (state))
        {
            SYS_WARNING ("Failed to connect: %s",
                         pa_strerror (pa_context_errno (m_priv->context)));
            break;
        }

        if (state == PA_CONTEXT_READY)
        {
            SYS_DEBUG ("initialized successfully");
            m_initialized = true;
            break;
        }

        pa_threaded_mainloop_wait (m_priv->mainloop);
    }

    pa_threaded_mainloop_unlock (m_priv->mainloop);

    if (m_initialized)
        get_default_sink_name ();
}

VolumeBarLogic::~VolumeBarLogic ()
{
    m_initialized = false;

    if (m_priv->mainloop)
        pa_threaded_mainloop_stop (m_priv->mainloop);

    if (m_priv->context)
    {
        pa_context_set_state_callback (m_priv->context, 0, 0);
        pa_context_unref (m_priv->context);
        m_priv->context = 0;
    }

    if (m_priv->mainloop)
    {
        pa_threaded_mainloop_free (m_priv->mainloop);
        m_priv->mainloop = 0;
    }

    if (m_priv->default_sink)
        delete m_priv->default_sink;

    delete m_priv;
}

static void
context_state_cb (pa_context *c, void *userdata)
{
    NsvVolume   *m_priv = (NsvVolume *) userdata;

    switch (pa_context_get_state (c))
    {
        case PA_CONTEXT_READY:
        case PA_CONTEXT_TERMINATED:
        case PA_CONTEXT_FAILED:
            pa_threaded_mainloop_signal (m_priv->mainloop, 0);
            break;

        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
            break;
    }
}

void
VolumeBarLogic::get_default_sink_name ()
{
    SYS_DEBUG ("");

    pa_operation *o = 0;

    pa_threaded_mainloop_lock (m_priv->mainloop);

    o = pa_context_get_server_info (m_priv->context, server_info_cb, (void *) m_priv);

    if (o)
    {
        while (pa_operation_get_state (o) == PA_OPERATION_RUNNING)
            pa_threaded_mainloop_wait (m_priv->mainloop);

        pa_operation_unref (o);
    }
    else
        SYS_WARNING ("pa_context_get_server_info failed: %s",
                     pa_strerror (pa_context_errno (m_priv->context)));

    pa_threaded_mainloop_unlock (m_priv->mainloop);
}

static void
server_info_cb (
    pa_context           *c,
    const pa_server_info *i,
    void                 *userdata)
{
    NsvVolume   *m_priv = (NsvVolume *) userdata;
    SYS_DEBUG ("%s", i->default_sink_name);

    if (m_priv->default_sink)
        delete m_priv->default_sink;

    m_priv->default_sink = new QString (i->default_sink_name);

    SYS_DEBUG ("Default sink = %s",
               qstring2char (m_priv->default_sink));

    pa_threaded_mainloop_signal (m_priv->mainloop, 0);
}

static void
sink_info_cb (
    pa_context          *c,
    const pa_sink_info  *i,
    int                  eol,
    void                *userdata)
{
    NsvVolume   *m_priv = (NsvVolume *) userdata;

    if (i == 0)
        return;

    m_priv->volume = pa_cvolume_avg (&i->volume);

    SYS_DEBUG ("Default volume = %d", m_priv->volume);

    pa_threaded_mainloop_signal (m_priv->mainloop, 0);
}

void
VolumeBarLogic::setVolume (double v)
{
    SYS_DEBUG ("%2.2f", v);

    if (m_initialized == false)
        return;

    pa_cvolume      vol;
    pa_operation   *o = 0;
    
    pa_threaded_mainloop_lock (m_priv->mainloop);

    m_priv->volume = (pa_volume_t) ((double) v * PA_VOLUME_NORM);
    pa_cvolume_set (&vol, 2, m_priv->volume);

    SYS_DEBUG ("Setting volume %f -> %d", v, m_priv->volume);
    o = pa_context_set_sink_volume_by_name (
                        m_priv->context,
                        qstring2char (m_priv->default_sink),
                        &vol, 0, 0);

    if (o)
        pa_operation_unref (o);
    else
        SYS_WARNING ("pa_context_set_sink_volume_by_name: %s",
                     pa_strerror (pa_context_errno (m_priv->context)));

    pa_threaded_mainloop_unlock (m_priv->mainloop);
}

double
VolumeBarLogic::getVolume ()
{
    if (m_initialized == false)
        return 0.0;

    pa_operation    *o = 0;
    pa_threaded_mainloop_lock (m_priv->mainloop);

    o = pa_context_get_sink_info_by_name (m_priv->context,
                                          qstring2char (m_priv->default_sink),
                                          sink_info_cb, (void *) m_priv);

    if (o)
    {
        while (pa_operation_get_state (o) == PA_OPERATION_RUNNING)
            pa_threaded_mainloop_wait (m_priv->mainloop);

        pa_operation_unref (o);
    }
    else
        SYS_WARNING ("pa_context_get_sink_info_by_name failed: %s",
                     pa_strerror (pa_context_errno (m_priv->context)));

    pa_threaded_mainloop_unlock (m_priv->mainloop);

    SYS_DEBUG ("%2.2f", (double) m_priv->volume / PA_VOLUME_NORM);

    return (double) m_priv->volume / PA_VOLUME_NORM;
}

