#ifndef NETWORKTRANSLATION_H
#define NETWORKTRANSLATION_H

#include <DuiLocale>

namespace DcpNetwork {    
    const QString PhoneNetworkText = trid("qtn_cell_phone_network", "Phone network");
    const QString RoamingTitleText = trid("qtn_cell_roaming", "Data roaming");
    const QString RoamingAlwaysAskText = trid("qtn_cell_always_ask", "Always ask");
    const QString RoamingAlwaysAllowText = trid("qtn_cell_always_allow", "Always allow");
    const QString RoamingUpdatesText = trid("qtn_cell_roaming_updates", "Allow also automatic connection usage");
    const QString DataCounterText = trid("qtn_cell_data_counter", "Data counter");
    const QString NetworkTitleText = trid("qtn_cell_network", "Network");
    const QString NetworkModeText = trid("qtn_cell_network_mode", "Network mode");    
    const QString NetworkSelectionText = trid("qtn_cell_network_selection", "Network selection");    
    const QString AvailableNetworksText = trid("qtn_cell_network_available", "Available networks");
    const QString NoAvailableNetworksText = trid("qtn_cell_no_networks_found", "No networks found");
    const QString NoNetworkSelectedText = trid("qtn_cell_no_network_selected", "No network selected. Change network selection to automatic?");
    const QString ChangeText = trid("qtn_cell_change", "Change");
    const QString HelpText = trid("qtn_comm_help", "Help");
    const QString DualText = trid("qtn_cell_network_dual", "Dual");
    const QString GSMText = trid("qtn_cell_network_gsm", "GSM");
    const QString ThreeGText = trid("qtn_cell_network_3g", "3G");
    const QString AutomaticText = trid("qtn_cell_automatic", "Automatic");
    const QString ManualText = trid("qtn_cell_manual", "Manual");
    const QString NoAccessText = trid("qtn_cell_error_no_access", "No access");
    const QString NoSIMText = trid("qtn_cell_error_no_sim", "Phone network is not available because there is no SIM card inserted.");

}
#endif // NETWORKTRANSLATION_H
