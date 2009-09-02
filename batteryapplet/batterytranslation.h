#ifndef DISPLAYTRANSLATION_H
#define DISPLAYTRANSLATION_H

#include <DuiLocale>

namespace DcpBattery {
    const QString AppletTitle = trid("NOT DEFINED", "Battery");
    const QString TalkTimeText = trid("qtn_ener_tt", "Talk time:");
    const QString TalkTimeValueText = trid("qtn_ener_ttms", "%1m%1h");
    const QString StandByTimeText = trid("qtn_ener_st", "Stand-by time:");
    const QString StandByTimeValueText = trid("qtn_ener_stms", "%1m%1h");
    const QString PSMText = trid("qtn_ener_ps", "Power save");
    const QString NBCText = trid("qtn_ener_nbc", "Normal battery consumption");
    const QString AutoPSMText = trid("qtn_ener_aps", "Automatic Power Saving");
    const QString AutoPSMDescText = trid("qtn_ener_sps", "Switch to energy optimized Power Save, when talk time is:");    
    const QString DisablePSMText = trid("qtn_ener_dsps", "Don’t switch automatically to power save");
    const QString AutoPSMAdv1Text = trid("qtn_ener_obc", "Optimized battery consumption:");
    const QString AutoPSMAdv2Text = trid("qtn_ener_dbsl", "Display brightness and the screen lights usage are reduced.");
    const QString AutoPSMAdv3Text = trid("qtn_ener_icr", "Internet connection and radios are activated only when needed");

};
#endif // DISPLAYTRANSLATION_H
