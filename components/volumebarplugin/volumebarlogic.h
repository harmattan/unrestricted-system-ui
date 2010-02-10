#ifndef VOLUMEBARLOGIC_H
#define VOLUMEBARLOGIC_H

typedef struct _NsvVolume NsvVolume; 

class VolumeBarLogic
{
    public:
        VolumeBarLogic ();
        ~VolumeBarLogic ();

        void setVolume (double v);
        double getVolume ();

    private:
        NsvVolume   *m_priv;
        bool         m_initialized;

        void get_default_sink_name ();
};

#endif
