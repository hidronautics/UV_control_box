#include "iserverdata.h"

#include <QDebug>

FromPult IServerData::generateFullMessage() {
    FromPult data;

    UVMutex.lock();
    data.controlData = UVState.control;
    data.cSMode = UVState.cSMode;
    data.controlContoursFlags = UVState.controlContoursFlags;
    data.thrusterPower = UVState.thrusterPower;
    data.resetImu = UVState.resetImu;
    UVMutex.unlock();

    return data;
}

void IServerData::parseFullMessage(ToPult message) {
    UVMutex.lock();
    UVState.imuData = message.imuData;
    UVState.depth = message.depth;
    UVState.connectionFlags = message.connectionFlags;
    UVMutex.unlock();
}