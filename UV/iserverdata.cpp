#include "iserverdata.h"

#include <QDebug>
#include <string>
#include <sstream>

int16_t resizeDoubleToInt16(double input);
int8_t resizeDoubleToInt8(double input);

uint16_t getCheckSumm16b(char *pcBlock, int len);
uint8_t isCheckSumm16bCorrect(char *pcBlock, int len);
void addCheckSumm16b(char *pcBlock, int len);

void set_bit(uint8_t &byte, uint8_t bit, bool state);

IServerData::IServerData()
        : IBasicData() {
    currentThruster = 0;
}

void IServerData::changeCurrentThruster(unsigned int slot) {
    if (slot < UVState.getThrusterAmount()) {
        currentThruster = slot;
    } else {
        std::string error = "Max thruster slot is: " +
                            std::to_string(UVState.getThrusterAmount() - 1) +
                            ", you are trying to change to:" +
                            std::to_string(slot);
        throw std::invalid_argument(error);
    }
}

int IServerData::getCurrentThruster() {
    return currentThruster;
}

int IServerData::getThrusterAmount() {
    int thrusterAmount;
    UVMutex.lock();
    thrusterAmount = UVState.getThrusterAmount();
    UVMutex.unlock();
    return thrusterAmount;
}

void IServerData::changeCurrentControlContour(unsigned int slot) {
    if (slot < UVState.getControlContourAmount()) {
        currentControlContour = slot;
    } else {
        std::string error = "Max thruster slot is: " +
                            std::to_string(UVState.getControlContourAmount() - 1) +
                            ", you are trying to change to:" +
                            std::to_string(slot);
        throw std::invalid_argument(error);
    }
}

int IServerData::getCurrentControlContour() {
    return currentControlContour;
}

int IServerData::getControlContourAmount() {
    int controlContourAmount;
    UVMutex.lock();
    controlContourAmount = UVState.getControlContourAmount();
    UVMutex.unlock();
    return controlContourAmount;
}

QByteArray IServerData::generateMessage(int message_type) {
    QByteArray formed;
    formed.clear();
    switch (message_type) {
        case MESSAGE_NORMAL:
            formed = generateNormalMessage();
            flashVmaSettings = false;
            break;
        case MESSAGE_CONFIG:
            formed = generateConfigMessage();
            break;
        case MESSAGE_DIRECT:
            formed = generateDirectMessage();
            break;
    }
    return formed;
}

void IServerData::setFlashVmaSettings(bool FlashVmaSettings) {
    flashVmaSettings = FlashVmaSettings;
}

QByteArray IServerData::generateNormalMessage() {
    QByteArray msg;
    msg.clear();
    QDataStream stream(&msg, QIODevice::Append);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    RequestNormalMessage req;
    fillStructure(req);

    stream << req.type;
    stream << req.flags;
    stream << req.march;
    stream << req.lag;
    stream << req.depth;
    stream << req.roll;
    stream << req.pitch;
    stream << req.yaw;
    for (int i = 0; i < DevAmount; i++) {
        stream << req.dev[i];
    }
    stream << req.lag_error;
    stream << req.dev_flags;
    stream << req.stabilize_flags;
    stream << req.cameras;
    stream << req.pc_reset;

    uint16_t checksum = getCheckSumm16b(msg.data(), msg.size());
    stream << checksum;

    return msg;
}

void IServerData::fillStructure(RequestNormalMessage &req) {
    req.flags = 0;

    UVMutex.lock();

    req.march = resizeDoubleToInt16(UVState.control.march);
    req.lag = resizeDoubleToInt16(UVState.control.lag);
    req.depth = resizeDoubleToInt16(UVState.control.depth);

    req.roll = resizeDoubleToInt16(UVState.control.roll);
    req.pitch = resizeDoubleToInt16(UVState.control.pitch);
    req.yaw = resizeDoubleToInt16(UVState.control.yaw);

    for (int i = 0; i < DevAmount; i++) {
        req.dev[i] = resizeDoubleToInt8(UVState.device[i].velocity);
    }

    req.lag_error = 0;

    req.dev_flags = 0;

    req.stabilize_flags = 0;

    qDebug() << "flashVmaSettings" << flashVmaSettings;
    set_bit(req.stabilize_flags, 6, flashVmaSettings);
    set_bit(req.stabilize_flags, 7, UVState.resetImu);

    req.cameras = 0;
    req.pc_reset = 0;

    UVMutex.unlock();
}

QByteArray IServerData::generateConfigMessage() {
    QByteArray msg;
    msg.clear();
    QDataStream stream(&msg, QIODevice::Append);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    RequestConfigMessage req;
    fillStructure(req);

    stream << req.type;
    stream << req.contour;
    stream << req.march;
    stream << req.lag;
    stream << req.depth;
    stream << req.roll;
    stream << req.pitch;
    stream << req.yaw;

    stream << req.pJoyUnitCast;
    stream << req.pSpeedDyn;
    stream << req.pErrGain;

    stream << req.posFilterT;
    stream << req.posFilterK;
    stream << req.speedFilterT;
    stream << req.speedFilterK;

    stream << req.pid_pGain;
    stream << req.pid_iGain;
    stream << req.pid_iMax;
    stream << req.pid_iMin;

    stream << req.pThrustersMin;
    stream << req.pThrustersMax;

    stream << req.thrustersFilterT;
    stream << req.thrustersFilterK;

    stream << req.sOutSummatorMax;
    stream << req.sOutSummatorMin;

    uint16_t checksum = getCheckSumm16b(msg.data(), msg.size());
    stream << checksum;

    return msg;
}

// TODO: deal with filling config message
void IServerData::fillStructure(RequestConfigMessage &req) {
    UVMutex.lock();

    req.march = resizeDoubleToInt16(UVState.control.march);
    req.lag = resizeDoubleToInt16(UVState.control.lag);
    req.depth = resizeDoubleToInt16(UVState.control.depth);
    req.roll = resizeDoubleToInt16(UVState.control.roll);
    req.pitch = resizeDoubleToInt16(UVState.control.pitch);
    req.yaw = resizeDoubleToInt16(UVState.control.yaw);

    req.contour = currentControlContour;

    req.pJoyUnitCast =      static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pJoyUnitCast     );
    req.pSpeedDyn =         static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pSpeedDyn        );
    req.pErrGain =          static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pErrGain         );
    req.posFilterT =        static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.posFilterT       );
    req.posFilterK =        static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.posFilterK       );
    req.speedFilterT =      static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.speedFilterT     );
    req.speedFilterK =      static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.speedFilterK     );
    req.pid_pGain =         static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pid_pGain        );
    req.pid_iGain =         static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pid_iGain        );
    req.pid_iMax =          static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pid_iMax         );
    req.pid_iMin =          static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pid_iMin         );
    req.pThrustersMin =     static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pThrustersMin    );
    req.pThrustersMax =     static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.pThrustersMax    );
    req.thrustersFilterT =  static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.thrustersFilterT );
    req.thrustersFilterK =  static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.thrustersFilterK );
    req.sOutSummatorMax =   static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.sOutSummatorMax  );
    req.sOutSummatorMin =   static_cast<int8_t>(UVState.controlContour[currentControlContour].constant.sOutSummatorMin  );

    UVMutex.unlock();
}

QByteArray IServerData::generateDirectMessage() {
    QByteArray msg;
    msg.clear();
    QDataStream stream(&msg, QIODevice::Append);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    RequestDirectMessage req;
    fillStructure(req);

    stream << req.type;
    stream << req.number;
    stream << req.id;
    stream << req.velocity;
    stream << req.reverse;
    stream << req.kForward;
    stream << req.kBackward;
    stream << req.sForward;
    stream << req.sBackward;
    qDebug() << "req.number" << req.number << "req.id" << req.id << "req.velocity" << req.velocity;

    uint16_t checksum = getCheckSumm16b(msg.data(), msg.size());
    stream << checksum;

    return msg;
}

void IServerData::fillStructure(RequestDirectMessage &req) {
    UVMutex.lock();

    req.number = currentThruster;
    req.id = static_cast<uint8_t>(UVState.thruster[req.number].id);

    if (UVState.thruster[req.number].power == false) {
        req.velocity = 0;
    } else {
        req.velocity = static_cast<int8_t>(UVState.thruster[req.number].velocity);
    }

    req.reverse = UVState.thruster[req.number].reverse;

    req.kForward = static_cast<int8_t>(UVState.thruster[req.number].kForward);
    req.kBackward = static_cast<int8_t>(UVState.thruster[req.number].kBackward);

    req.sForward = static_cast<int8_t>(UVState.thruster[req.number].sForward);
    req.sBackward = static_cast<int8_t>(UVState.thruster[req.number].sBackward);

    UVMutex.unlock();
}

void IServerData::parseMessage(QByteArray message, int message_type) {
    switch (message_type) {
        case MESSAGE_NORMAL:
            parseNormalMessage(message);
            break;
        case MESSAGE_CONFIG:
            parseConfigMessage(message);
            break;
        case MESSAGE_DIRECT:
            parseDirectMessage(message);
            break;
        default:
            std::stringstream stream;
            stream << "Incorrect message type: [" << message_type << "]";
            throw std::invalid_argument(stream.str());
    }
}

void IServerData::parseNormalMessage(QByteArray msg) {
    ResponseNormalMessage res;

    uint16_t checksum_calc = getCheckSumm16b(msg.data(), msg.size() - 2);

    QDataStream stream(&msg, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    stream >> res.roll;
    stream >> res.pitch;
    stream >> res.yaw;

    stream >> res.rollSpeed;
    stream >> res.pitchSpeed;
    stream >> res.yawSpeed;

    stream >> res.depth;
    stream >> res.inpressure;

    stream >> res.dev_state;
    stream >> res.leak_data;

    for (int i = 0; i < VmaAmount; i++) {
        stream >> res.vma_current[i];
    }

    for (int i = 0; i < DevAmount; i++) {
        stream >> res.dev_current[i];
    }

    stream >> res.vma_errors;
    stream >> res.dev_errors;
    stream >> res.pc_errors;

    stream >> res.checksum;

    if (res.checksum != checksum_calc) {
        std::stringstream stream;
        stream << "[ISERVERDATA] Checksum is invalid. Calculated: [" <<
               std::ios::hex << checksum_calc << "] " <<
               "Received: [" <<
               std::ios::hex << res.checksum << "]";
        throw std::invalid_argument(stream.str());
    }

    pullFromStructure(res);
}

void IServerData::pullFromStructure(ResponseNormalMessage res) {
    UVMutex.lock();

    UVState.imu.roll = static_cast<double>(res.roll);
    UVState.imu.pitch = static_cast<double>(res.pitch);
    UVState.imu.yaw = static_cast<double>(res.yaw);

    UVState.imu.rollSpeed = static_cast<double>(res.rollSpeed);
    UVState.imu.pitchSpeed = static_cast<double>(res.pitchSpeed);
    UVState.imu.yawSpeed = static_cast<double>(res.yawSpeed);

    UVState.imu.depth = static_cast<double>(res.depth);
    UVState.aux_inpressure = static_cast<double>(res.inpressure);

    /*
    uint8_t dev_state;
    int16_t leak_data;

    uint16_t vma_current[VmaAmount];
    uint16_t dev_current[DevAmount];

    uint16_t vma_errors;
    uint16_t dev_errors;
    uint8_t pc_errors;
    */

    UVMutex.unlock();
}

void IServerData::parseConfigMessage(QByteArray msg) {
    ResponseConfigMessage res;

    uint16_t checksum_calc = getCheckSumm16b(msg.data(), msg.size() - 2);

    QDataStream stream(&msg, QIODevice::ReadOnly);

    stream >> res.code;

    stream >> res.roll;
    stream >> res.pitch;
    stream >> res.yaw;
    stream >> res.raw_yaw;

    stream >> res.rollSpeed;
    stream >> res.pitchSpeed;
    stream >> res.yawSpeed;

    stream >> res.pressure;
    stream >> res.in_pressure;

    stream >> res.inputSignal;
    stream >> res.speedSignal;
    stream >> res.posSignal;

    stream >> res.joyUnitCasted;
    stream >> res.joy_iValue;
    stream >> res.posError;
    stream >> res.speedError;
    stream >> res.dynSummator;
    stream >> res.pidValue;
    stream >> res.posErrorAmp;
    stream >> res.speedFiltered;
    stream >> res.posFiltered;
    stream >> res.pid_iValue;
    stream >> res.thrustersFiltered;
    stream >> res.outputSignal;

    stream >> res.checksum;

    if (res.checksum != checksum_calc) {
        std::stringstream stream;
        stream << "[ISERVERDATA] Checksum is invalid. Calculated: [" <<
               std::ios::hex << checksum_calc << "] " <<
               "Received: [" <<
               std::ios::hex << res.checksum << "]";
        throw std::invalid_argument(stream.str());
    }

    pullFromStructure(res);
}

// TODO finish responseconfigmessage structure pulling
void IServerData::pullFromStructure(ResponseConfigMessage res) {
    UVMutex.lock();

    UVState.imu.roll = res.roll;
    UVState.imu.pitch = res.pitch;
    UVState.imu.yaw = res.yaw;
    UVState.imu.rollSpeed = res.rollSpeed;
    UVState.imu.pitchSpeed = res.pitchSpeed;
    UVState.imu.yawSpeed = res.yawSpeed;

    UVState.controlContour[currentControlContour].state.inputSignal = res.inputSignal;
    UVState.controlContour[currentControlContour].state.speedSignal = res.speedSignal;
    UVState.controlContour[currentControlContour].state.posSignal = res.posSignal;

    UVState.controlContour[currentControlContour].state.joyUnitCasted = res.joyUnitCasted;
    UVState.controlContour[currentControlContour].state.joy_iValue = res.joy_iValue;
    UVState.controlContour[currentControlContour].state.posError = res.posError;
    UVState.controlContour[currentControlContour].state.speedError = res.speedError;
    UVState.controlContour[currentControlContour].state.dynSummator = res.dynSummator;
    UVState.controlContour[currentControlContour].state.pidValue = res.pidValue;
    UVState.controlContour[currentControlContour].state.posErrorAmp = res.posErrorAmp;
    UVState.controlContour[currentControlContour].state.speedFiltered = res.speedFiltered;
    UVState.controlContour[currentControlContour].state.posFiltered = res.posFiltered;
    UVState.controlContour[currentControlContour].state.pid_iValue = res.pid_iValue;
    UVState.controlContour[currentControlContour].state.thrustersFiltered = res.thrustersFiltered;

    UVState.controlContour[currentControlContour].state.outputSignal = res.outputSignal;

    UVMutex.unlock();
}

void IServerData::parseDirectMessage(QByteArray msg) {
    ResponseDirectMessage res;

    uint16_t checksum_calc = getCheckSumm16b(msg.data(), msg.size() - 2);

    QDataStream stream(&msg, QIODevice::ReadOnly);

    stream >> res.number;
    stream >> res.connection;
    stream >> res.current;

    stream >> res.checksum;

    if (res.checksum != checksum_calc) {
        std::stringstream stream;
        stream << "[ISERVERDATA] Checksum is invalid. Calculated: [" <<
               std::ios::hex << checksum_calc << "] " <<
               "Received: [" <<
               std::ios::hex << res.checksum << "]";
        throw std::invalid_argument(stream.str());
    }

    pullFromStructure(res);
}

// TODO finish ResponseDirectMessage structure pulling
void IServerData::pullFromStructure(ResponseDirectMessage res) {
    res.number = 9;
//    res.connection;
//    res.current;
}

int16_t resizeDoubleToInt16(double input) {
    int16_t output = 0;
    output = static_cast<int16_t>(input);
    return output;
}

int8_t resizeDoubleToInt8(double input) {
    int16_t output = 0;
    output = static_cast<int8_t>(input);
    return output;
}

/* CRC16-CCITT algorithm */
uint16_t getCheckSumm16b(char *pcBlock, int len) {
    uint16_t crc = 0xFFFF;
    //int crc_fix = reinterpret_cast<int*>(&crc);
    uint8_t i;

    while (len--) {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

uint8_t isCheckSumm16bCorrect(char *pcBlock, int len) {
    uint16_t crc_calculated = getCheckSumm16b(pcBlock, len);

    uint16_t *crc_pointer = reinterpret_cast<uint16_t *>(&pcBlock[len - 2]);
    uint16_t crc_got = *crc_pointer;

    if (crc_got == crc_calculated) {
        return true;
    } else {
        return false;
    }
}

void addCheckSumm16b(char *pcBlock, int len) {
    uint16_t crc = getCheckSumm16b(pcBlock, len);
    uint16_t *crc_pointer = reinterpret_cast<uint16_t *>(&pcBlock[len - 2]);
    *crc_pointer = crc;
}

void set_bit(uint8_t &byte, uint8_t bit, bool state) {
    uint8_t value = 1;
    if (state) {
        byte = byte | (value << bit);
    } else {
        byte = byte & ~(value << bit);
    }
}


