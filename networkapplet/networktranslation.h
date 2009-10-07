#ifndef NETWORKTRANSLATION_H
#define NETWORKTRANSLATION_H

#include <DuiLocale>

namespace DcpNetwork {    
    const QString PhoneNetworkText = trid("qtn_cell_phone_network", "Phone network");
    const QString RoamingText = trid("qtn_cell_roaming", "Roaming");
    const QString RoamingButtonText = trid("qtn_cell_enable_roaming", "Enable roaming");
    const QString RoamingUpdatesButtonText = trid("qtn_cell_roaming_updates", "Enable automatic updates");
    const QString DataCounterText = trid("qtn_cell_data_counter", "Data counter");
    const QString NetworkText = trid("qtn_cell_network", "Network");
    const QString NetworkModeText = trid("qtn_cell_network_mode", "Network mode");    
    const QString NetworkSelectionText = trid("qtn_cell_network_selection", "Network selection");    
    const QString AvailableNetworksText = trid("qtn_cell_network_available", "Available networks");
    const QString NoAvailableNetworksText = trid("qtn_cell_no_networks_found", "No networks found");
    const QString NoNetworkSelectedText = trid("qtn_cell_no_network_selected", "No network selected. Change network selection to automatic?");
    const QString ChangeText = trid("qtn_cell_change", "Change");

}
#endif // NETWORKTRANSLATION_H
