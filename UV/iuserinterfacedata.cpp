#include "iuserinterfacedata.h"

#include <stdexcept>

IUserInterfaceData::IUserInterfaceData()
        : IBasicData() {

}

ControlData IUserInterfaceData::getControlData() {
    ControlData data;

    UVMutex.lock();
    data = UVState.control;
    UVMutex.unlock();

    return data;
}

double  IUserInterfaceData::getDepth() {
    double data;

    UVMutex.lock();
    data = UVState.depth;
    UVMutex.unlock();

    return data;
}

ImuData IUserInterfaceData::getImuData() {
    ImuData data;

    UVMutex.lock();
    data = UVState.imuData;
    UVMutex.unlock();

    return data;
}

MoutionDataAfterAlgoritms IUserInterfaceData::getMoutionDataAfterAlgoritms(){
    MoutionDataAfterAlgoritms data;

    UVMutex.lock();
    data = UVState.moutionDataAfterAlgoritms;
    UVMutex.unlock();

    return data;
}

void IUserInterfaceData::setResetImuFlag(bool value) {
    UVMutex.lock();
    UVState.resetImu = value;
    UVMutex.unlock();
}

double IUserInterfaceData::getThrusterCurrent(int id) {
    double data;

    UVMutex.lock();
    data = UVState.thruster[id].current;
    UVMutex.unlock();

    return data;
}

void IUserInterfaceData::setThrusterVelocity(int id, double value) {
    UVMutex.lock();
    UVState.thruster[id].velocity = value;
    UVMutex.unlock();
}

void IUserInterfaceData::setControlContoursFlags(int contour, bool value) {

}

ConnectionFlags IUserInterfaceData::getConnectionFlags() {
    ConnectionFlags data;

    UVMutex.lock();
    data = UVState.connectionFlags;
    UVMutex.unlock();

    return data;
}

void IUserInterfaceData::SetThrusterPowerFlag(bool value) {
    UVMutex.lock();
    UVState.thrusterPower = value;
    UVMutex.unlock();
}

