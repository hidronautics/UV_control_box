#ifndef UV_STATE_H
#define UV_STATE_H

#include "stdint.h"

#include <QDebug>

enum e_CSMode {
    MODE_HANDLE = 0,
    MODE_IDENTIFICATION_1,
    MODE_IDENTIFICATION_2,
    MODE_AUTO,
    MODE_THRUSTER
};

enum e_StabilizationContours {
    CONTOUR_DEPTH = 0,
    CONTOUR_MARCH,
    CONTOUR_LAG,
    CONTOUR_YAW,
    CONTOUR_ROLL,
    CONTOUR_PITCH
};

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
    bool pitch;
    bool roll;
    bool march;
    bool depth;
    bool lag;
};

struct ImuData {
    ImuData();
    float ax;
    float ay;
    float az;

    float wx;
    float wy;
    float wz;

    float psi;
    float teta;
    float gamma;

    float q0;
    float q1;
    float q2;
    float q3;
};

struct MoutionDataAfterAlgoritms {
    MoutionDataAfterAlgoritms();
    float ax;
    float ay;
    float az;

    float vx;
    float vy;
    float vz;

    float vxg;
    float vyg;
    float vzg;

    float x;
    float y;
    float z;

    float wx;
    float wy;
    float wz;

    float dPsi;
    float dTeta;
    float dGamma;

    float psi;
    float teta;
    float gamma;

    float q0;
    float q1;
    float q2;
    float q3;
};

struct Thruster {
    Thruster();
    int id;
    float velocity;
    float current;
};

struct ConnectionFlags {
    ConnectionFlags();
    bool vectorNav;
    bool joystick;
    bool controlSystem;
    bool thrusterController;
};

class UV_State {
public:
    UV_State();
    ~UV_State();

    ControlData control;

    ControlContoursFlags controlContoursFlags;
    e_CSMode cSMode;

    ImuData imuData;
    MoutionDataAfterAlgoritms moutionDataAfterAlgoritms;
    float depth;

    Thruster *thruster;

    // Flags
    ConnectionFlags connectionFlags;
    bool resetImu;
    bool thrusterPower;
};

#endif // UV_STATE_H
