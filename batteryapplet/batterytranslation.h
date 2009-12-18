#ifndef DISPLAYTRANSLATION_H
#define DISPLAYTRANSLATION_H

#include <DuiLocale>

namespace DcpBattery
{
const QString AppletTitle = trid("qtn_ener_battery", "Battery");
const QString HelpText = trid("qtn_comm_help", "Help");
const QString TalkTimeText = trid("qtn_ener_tt", "Estimated talk time:");
const QString StandByTimeText = trid("qtn_ener_st", "Estimated stand-by time:");
const QString TimeValueText = trid("qtn_ener_remtime", "%a hours %b minutes");
const QString PSMAutoActivateText = trid("qtn_ener_autops", "Auto activate power save");
const QString PSMActivateText = trid("qtn_ener_aps", "Activate power save now");
const QString PSMDeactivateText = trid("qtn_ener_dps", "Deactivate power save now");
}
#endif // DISPLAYTRANSLATION_H
