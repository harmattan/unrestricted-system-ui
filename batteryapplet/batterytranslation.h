#ifndef DISPLAYTRANSLATION_H
#define DISPLAYTRANSLATION_H

#include <DuiLocale>

namespace DcpBattery {
    const QString AppletTitle = trid("NOT DEFINED", "Battery");
    const QString TalkTimeText = trid("qtn_ener_tt", "Estimated talk time:");    
    const QString StandByTimeText = trid("qtn_ener_st", "Estimated stand-by time:");
    const QString TimeValueText = trid("qtn_ener_remtime", "%a hours %b minutes");    
    const QString PSMAutoActivateText = trid("qtn_ener_autops", "Auto activate power save");
}
#endif // DISPLAYTRANSLATION_H
