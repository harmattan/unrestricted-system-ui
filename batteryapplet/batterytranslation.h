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
    const QString DisablePSMText = trid("qtn_ener_dsps", "Do not switch automatically to power save");
    const QString AutoPSMAdv1Text = trid("qtn_ener_obc", "Optimized battery consumption:");
    const QString AutoPSMAdv2Text = trid("qtn_ener_dbsl", "Display brightness and the screen lights usage are reduced.");
    const QString AutoPSMAdv3Text = trid("qtn_ener_icr", "Internet connection and radios are activated only when needed");
    const QString BatteryImageFull100 = trid("dui_ener_s_full100", "");
    const QString BatteryImageFull75 = trid("dui_ener_s_full75", "");
    const QString BatteryImageFull50 = trid("dui_ener_s_full50", "");
    const QString BatteryImageFull25 = trid("dui_ener_s_full25", "");
    const QString BatteryImageLow = trid("dui_ener_s_low", "");
    const QString BatteryImageVeryLow = trid("dui_ener_s_verylow", "");
    const QString BatteryImageChargingFull100 = trid("dui_ener_m_full100", "");
    const QString BatteryImageChargingFull75 = trid("dui_ener_m_full75", "");
    const QString BatteryImageChargingFull50 = trid("dui_ener_m_full50", "");
    const QString BatteryImageChargingFull25 = trid("dui_ener_m_full25", "");
    const QString BatteryImageChargingLow = trid("dui_ener_m_low", "");
    const QString BatteryImageChargingVeryLow = trid("dui_ener_m_verylow", "");

};
#endif // DISPLAYTRANSLATION_H
