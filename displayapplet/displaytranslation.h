#ifndef DISPLAYTRANSLATION_H
#define DISPLAYTRANSLATION_H

#include <DuiLocale>

namespace DcpDisplay {
    const QString AppletTitle = trid("NOT DEFINED", "Display");
    const QString BrightnessText = trid("NOT_DEFINED", "Brightness:");
    const QString BlankTimeoutText = trid("NOT_DEFINED", "Screen lights off after:");
    const QString DimTimeoutText = trid("NOT_DEFINED", "Screen dimmed after:");
    const QString BlankTimeoutValueText = trid("NOT_DEFINED", "%1sec");
    const QString DimTimeoutValueText = trid("NOT_DEFINED", "%1sec");
    const QString BlankInhibitButtonText = trid("NOT_DEFINED", "Screen lights on during the charging");
    const QString NoteText = trid("NOT_DEFINED", "In the power save mode, screen light settings are affected.");

};
#endif // DISPLAYTRANSLATION_H
