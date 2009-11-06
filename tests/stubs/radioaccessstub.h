#ifndef RADIOACCESSSTUB_H
#define RADIOACCESSSTUB_H

#include <QObject>

namespace Cellular
{



class RadioAccess : public QObject
{
    Q_OBJECT

public:
    RadioAccess(QObject *parent=0);
    ~RadioAccess();

    enum Mode
    {
        UnknownMode = -1, //!< Radio access selection mode unknown
        DualMode, //!< Dual mode access selection in use
        GSMMode, //!< GSM only access selection mode in use
        UMTSMode //!< UMTS only access selection mode in use
    };

    //! Get current access mode
    RadioAccess::Mode mode() const;

public slots:
    //! Set radio access selection mode
    void setMode(RadioAccess::Mode mode);

signals:
    //! Radio access selection mode changed
    void modeChanged(int mode);

};

}
#endif // RADIOACCESSSTUB_H
