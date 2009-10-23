#ifndef DCPCALLANDSIM_H
#define DCPCALLANDSIM_H

namespace DcpCallAndSim
{
enum {
    None = -1,
    Main = 0
};

enum Data {
    CallerIdSendingData = 0,
    CallWaitingData,
    CallForwardingData,
    PinRequestData,
    AllData
};
}

#endif // DCPCALLANDSIM_H
