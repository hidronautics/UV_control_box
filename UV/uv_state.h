#ifndef UV_STATE_H
#define UV_STATE_H

#include "stdint.h"

#include <QDebug>

struct ControlData {
    ControlData();
    double yaw;
    double pitch;
    double roll;
    double march;
    double depth;
    double lag;
};

struct ControlContoursFlags {
    ControlContoursFlags();
    bool yaw;
    bool pitch;w
    bool roll;
    bool march;
    bool depth;
    bool lag;
};

struct ImuData {
    ImuData();
    double ax;
    double ay;
    double az;

    double wx;
    double wy;
    double wz;

    double psi;
    double teta;
    double gamma;

    double q0;
    double q1;
    double q2;
    double q3;
};

struct MoutionDataAfterAlgoritms {
    MoutionDataAfterAlgoritms();
    double ax;
    double ay;
    double az;

    double vx;
    double vy;
    double vz;

    double vxg;
    double vyg;
    double vzg;

    double x;
    double y;
    double z;

    double wx;
    double wy;
    double wz;

    double dPsi;
    double dTeta;
    double dGamma;

    double psi;
    double teta;
    double gamma;

    double q0;
    double q1;
    double q2;
    double q3;
};

struct Thruster {
    Thruster();
    int id;
    double velocity;
    double current;
};

struct ConnectionFlags {
    ConnectionFlags();
    bool vectorNav;
    bool joystick;
    bool controlSystem;
    bool WMAcontroller;
};

class UV_State {
public:
    UV_State();
    ~UV_State();

    ControlData control;

    ControlContoursFlags controlContoursFlags;

    ImuData imuData;
    MoutionDataAfterAlgoritms moutionDataAfterAlgoritms;
    double depth;

    Thruster *thruster;

    // Flags
    ConnectionFlags connectionFlags;
    bool resetImu;
    bool thrusterPower;
};

#endif // UV_STATE_H
