#ifndef DISPLAYTRANSLATION_H
#define DISPLAYTRANSLATION_H

#include <DuiLocale>

namespace DcpDisplay {
    const QString AppletTitle = trid("NOT DEFINED", "Display");
    const QString HelpText = trid("NOT_DEFINED", "Help");
    const QString BrightnessText = trid("qtn_disp_bright", "Brightness:");
    const QString ScreenLightsText = trid("qtn_disp_screenoff", "Screen lights off after:");
    const QString ScreenLightsValueText = trid("NOT_DEFINED", "%1sec");    
    const QString BlankInhibitText = trid("qtn_disp_screenon", "Keep screen lights on during the charging");
    const QString NoteText = trid("qtn_disp_note", "Note! In the power save mode, screen light settings are affected.");

}
#endif // DISPLAYTRANSLATION_H
