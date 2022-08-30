#include "iuserinterfacedata.h"

#include <stdexcept>

IUserInterfaceData::IUserInterfaceData()
        : IBasicData() {
    setControlContoursFlags(e_StabilizationContours::CONTOUR_ROLL, true);
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

void IUserInterfaceData::setControlContoursFlags(e_StabilizationContours contour, bool value) {
    switch (contour) {
        case CONTOUR_DEPTH:
            qDebug() << "CONTOUR_DEPTH setted to" << value;
            UVMutex.lock();
            UVState.controlContoursFlags.depth = value;
            UVMutex.unlock();

        case CONTOUR_LAG:
            UVMutex.lock();
            UVState.controlContoursFlags.lag = value;
            UVMutex.unlock();

        case CONTOUR_MARCH:
            UVMutex.lock();
            UVState.controlContoursFlags.march = value;
            UVMutex.unlock();

        case CONTOUR_PITCH:
            UVMutex.lock();
            UVState.controlContoursFlags.pitch = value;
            UVMutex.unlock();

        case CONTOUR_ROLL:
            UVMutex.lock();
            UVState.controlContoursFlags.roll = value;
            UVMutex.unlock();

        case CONTOUR_YAW:
            UVMutex.lock();
            UVState.controlContoursFlags.yaw = value;
            UVMutex.unlock();
    }
}

void IUserInterfaceData::setCSMode(e_CSMode mode) {
    qDebug() << "CSMode" << mode;
    UVMutex.lock();
    UVState.cSMode = mode;
    UVMutex.unlock();
}

ConnectionFlags IUserInterfaceData::getConnectionFlags() {
    ConnectionFlags data;

    UVMutex.lock();
    data = UVState.connectionFlags;
    UVMutex.unlock();

    return data;
}

void IUserInterfaceData::setThrusterPowerFlag(bool value) {
    UVMutex.lock();
    UVState.thrusterPower = value;
    UVMutex.unlock();
}
