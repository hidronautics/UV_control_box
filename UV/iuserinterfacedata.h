#ifndef IUSERINTERFACEDATA_H
#define IUSERINTERFACEDATA_H

#include "ibasicdata.h"

class IUserInterfaceData : public IBasicData {
public:
    IUserInterfaceData();

    ControlData getControlData();
    double getDepth();
    ImuData getImuData();
    MoutionDataAfterAlgoritms getMoutionDataAfterAlgoritms();

    double getThrusterCurrent(int id);
    void setThrusterVelocity(int id, double value);

    void setControlContoursFlags(int contour, bool value);

    ConnectionFlags getConnectionFlags();
    void setResetImuFlag(bool value);
    void SetThrusterPowerFlag(bool value);
};

#endif // IUSERINTERFACEDATA_H
