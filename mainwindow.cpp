#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->radioButton_cSMode_auto,            SIGNAL(clicked()), this, SLOT(cSModeChange_auto()));
    connect(ui->radioButton_cSMode_identification1, SIGNAL(clicked()), this, SLOT(cSModeChange_identification1()));
    connect(ui->radioButton_cSMode_identification2, SIGNAL(clicked()), this, SLOT(cSModeChange_identification2()));
    connect(ui->radioButton_cSMode_handle,          SIGNAL(clicked()), this, SLOT(cSModeChange_handle()));
    connect(ui->checkBox_cSMode_thruster, SIGNAL(toggled(bool)), this, SLOT(cSModeChange_thruster(bool)));

    connect(ui->pushButton_thrusterPower, SIGNAL(toggled(bool)), this, SLOT(powerThrusterToggled(bool)));

    connect(ui->checkBox_stabilizeYaw   , SIGNAL(toggled(bool)), this, SLOT(stabilizeYawToggled(bool)));
    connect(ui->checkBox_stabilizePitch , SIGNAL(toggled(bool)), this, SLOT(stabilizePitchToggled(bool)));
    connect(ui->checkBox_stabilizeRoll  , SIGNAL(toggled(bool)), this, SLOT(stabilizeRollToggled(bool)));
    connect(ui->checkBox_stabilizeMarch , SIGNAL(toggled(bool)), this, SLOT(stabilizeMarchToggled(bool)));
    connect(ui->checkBox_stabilizeDepth , SIGNAL(toggled(bool)), this, SLOT(stabilizeDepthToggled(bool)));
    connect(ui->checkBox_stabilizeLag   , SIGNAL(toggled(bool)), this, SLOT(stabilizeLagToggled(bool)));

    connect(ui->pushButton_typicalInput_start, SIGNAL(clicked()), this, SLOT(typicalInput_start()));
    connect(ui->pushButton_typicalInput_stop, SIGNAL(clicked()), this, SLOT(typicalInput_stopByButton()));
    ui->pushButton_typicalInput_stop->setEnabled(false);

    pultProtocol = new Pult::PC_Protocol(QHostAddress("192.168.4.1"), 13021, QHostAddress("192.168.4.2"),
                                          13020, 10);

    qDebug() << "-----start exchange";
    pultProtocol->startExchange();

    connect(pultProtocol, SIGNAL(dataReceived()), this, SLOT(updateUi_fromROV()));
    connect(this, SIGNAL(updateCompass(double)), ui->compassFrame, SLOT(setYaw(double)));
    connect(this, SIGNAL(updateDepth(int)), ui->progressBar_depth, SLOT(setValue(int)));
    connect(this, SIGNAL(updateDepth(double)), ui->lable_depth, SLOT(setNum(double)));

    joystick = new Joystick("Joystick", 10, 0);
    step = new Step();
    gradation = new Gradation();
    connect(this->step, SIGNAL(stop_signalByTiemr()), this, SLOT(typicalInput_stopByTiemr()));
    connect(this->gradation, SIGNAL(stop_signalByTiemr()), this, SLOT(typicalInput_stopByTiemr()));


//    connect(joystick, SIGNAL(controlChanged()), this, SLOT(updateUi_fromControl()));
    updateControl_timer = new QTimer(this);
    connect(updateControl_timer, SIGNAL(timeout()), this, SLOT(updateUi_fromControl()));
    updateControl_timer->start(10);

    ui->pushButton_thrusterPower->toggled(true);
//    ui->pushButton_thrusterPower->f;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUi_fromControl() {
    ControlData control = uv_interface.getControlData();

    ui->label_control_march     ->setText(QString::number(control.march,    'f', 2));
    ui->label_control_lag       ->setText(QString::number(control.lag,      'f', 2));
    ui->label_control_depth     ->setText(QString::number(control.depth,    'f', 2));
    ui->label_control_yaw       ->setText(QString::number(control.yaw,      'f', 2));
    ui->label_control_roll      ->setText(QString::number(control.roll,     'f', 2));
    ui->label_control_pitch     ->setText(QString::number(control.pitch,    'f', 2));
}

void MainWindow::updateUi_fromROV() {
    ImuData imuData = uv_interface.getImuData();
    float depth = uv_interface.getDepth();

    ui->label_vectorNav_ax      ->setText(QString::number(imuData.ax,       'f', 2));
    ui->label_vectorNav_ay      ->setText(QString::number(imuData.ay,       'f', 2));
    ui->label_vectorNav_az      ->setText(QString::number(imuData.az,       'f', 2));
    ui->label_vectorNav_wx      ->setText(QString::number(imuData.wx,       'f', 2));
    ui->label_vectorNav_wy      ->setText(QString::number(imuData.wy,       'f', 2));
    ui->label_vectorNav_wz      ->setText(QString::number(imuData.wz,       'f', 2));
    ui->label_vectorNav_psi     ->setText(QString::number(imuData.psi,      'f', 2));
    ui->label_vectorNav_teta    ->setText(QString::number(imuData.teta,     'f', 2));
    ui->label_vectorNav_gamma   ->setText(QString::number(imuData.gamma,    'f', 2));
    ui->label_vectorNav_q0      ->setText(QString::number(imuData.q0,       'f', 2));
    ui->label_vectorNav_q1      ->setText(QString::number(imuData.q1,       'f', 2));
    ui->label_vectorNav_q2      ->setText(QString::number(imuData.q2,       'f', 2));
    ui->label_vectorNav_q3      ->setText(QString::number(imuData.q3,       'f', 2));

    emit(updateCompass(imuData.psi));
    emit(updateDepth(int(depth*100)));
    emit(updateDepth(double(depth)));
}

void MainWindow::cSModeChange_auto() {
    uv_interface.setCSMode(e_CSMode::MODE_AUTO);
}

void MainWindow::cSModeChange_identification1() {
    uv_interface.setCSMode(e_CSMode::MODE_IDENTIFICATION_1);
}

void MainWindow::cSModeChange_identification2() {
    uv_interface.setCSMode(e_CSMode::MODE_IDENTIFICATION_2);
}

void MainWindow::cSModeChange_handle() {
    uv_interface.setCSMode(e_CSMode::MODE_HANDLE);
}

void MainWindow::cSModeChange_thruster(bool checked) {
    checked ? cSModeChange_thrusterOn() : cSModeChange_thrusterOff();
}

void MainWindow::cSModeChange_thrusterOn() {
    ui->radioButton_cSMode_auto             ->setEnabled(false);
    ui->radioButton_cSMode_identification1  ->setEnabled(false);
    ui->radioButton_cSMode_identification2  ->setEnabled(false);
    ui->radioButton_cSMode_handle           ->setEnabled(false);

    uv_interface.setCSMode(e_CSMode::MODE_THRUSTER);
}

void MainWindow::cSModeChange_thrusterOff() {
    ui->radioButton_cSMode_auto             ->setEnabled(true);
    ui->radioButton_cSMode_identification1  ->setEnabled(true);
    ui->radioButton_cSMode_identification2  ->setEnabled(true);
    ui->radioButton_cSMode_handle           ->setEnabled(true);

    if (ui->radioButton_cSMode_auto             ->isChecked() == true) uv_interface.setCSMode(e_CSMode::MODE_AUTO);
    if (ui->radioButton_cSMode_identification1  ->isChecked() == true) uv_interface.setCSMode(e_CSMode::MODE_IDENTIFICATION_1);
    if (ui->radioButton_cSMode_identification2  ->isChecked() == true) uv_interface.setCSMode(e_CSMode::MODE_IDENTIFICATION_2);
    if (ui->radioButton_cSMode_handle           ->isChecked() == true) uv_interface.setCSMode(e_CSMode::MODE_HANDLE);
}

void MainWindow::powerThrusterToggled(bool state) {
    uv_interface.setThrusterPowerFlag(state);
    state ? ui->pushButton_thrusterPower->setText("ВМА ВЫКЛючены") : ui->pushButton_thrusterPower->setText("ВМА ВКЛючены");

    qDebug() << "setThrusterPowerFlag" << state;
}

void MainWindow::stabilizeYawToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_YAW, state);
}

void MainWindow::stabilizePitchToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_PITCH, state);
}

void MainWindow::stabilizeRollToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_ROLL, state);
}

void MainWindow::stabilizeMarchToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_MARCH, state);
}

void MainWindow::stabilizeDepthToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_DEPTH, state);
}

void MainWindow::stabilizeLagToggled(bool state) {
    uv_interface.setControlContoursFlags(e_StabilizationContours::CONTOUR_LAG, state);
}

void MainWindow::typicalInput_start() {
    joystick->setOff();
    ui->pushButton_typicalInput_start->setEnabled(false);
    ui->pushButton_typicalInput_stop->setEnabled(true);

    ui->sdoubleSpinBox_typicalInput_T->         setEnabled(false);
    ui->doubleSpinBox_typicalInput_k_1->        setEnabled(false);
    ui->doubleSpinBox_typicalInput_k_2->        setEnabled(false);
    ui->doubleSpinBox_typicalInput_k_3->        setEnabled(false);
    ui->doubleSpinBox_typicalInput_T1->         setEnabled(false);
    ui->doubleSpinBox_typicalInput_T2->         setEnabled(false);
    ui->doubleSpinBox_typicalInput_T3->         setEnabled(false);
    ui->comboBox_typicalInput_selectedContour-> setEnabled(false);
    ui->comboBox_typicalInput_shape->           setEnabled(false);

    float k_1 =  ui->doubleSpinBox_typicalInput_k_1->value();
    float k_2 =  ui->doubleSpinBox_typicalInput_k_2->value();
    float k_3 =  ui->doubleSpinBox_typicalInput_k_3->value();
    int T1 =  ui->doubleSpinBox_typicalInput_T1->value() * 1000;
    int T2 =  ui->doubleSpinBox_typicalInput_T2->value() * 1000;
    int T3 =  ui->doubleSpinBox_typicalInput_T3->value() * 1000;
    int T =  ui->sdoubleSpinBox_typicalInput_T->value() * 1000;
    auto direction = ControlBase::e_actionTypes::SET_YAW;

    switch (ui->comboBox_typicalInput_selectedContour->currentIndex()) {
        case 0:
            direction = ControlBase::e_actionTypes::SET_YAW;
            break;
        case 1:
            direction = ControlBase::e_actionTypes::SET_PITCH;
            break;
        case 2:
            direction = ControlBase::e_actionTypes::SET_ROLL;
            break;
        case 3:
            direction = ControlBase::e_actionTypes::SET_MARCH;
            break;
        case 4:
            direction = ControlBase::e_actionTypes::SET_LAG;
            break;
        case 5:
            direction = ControlBase::e_actionTypes::SET_DEPTH;
            break;
    }
    uv_interface.setExperimentTypicalInputFlag(true);

    switch (ui->comboBox_typicalInput_shape->currentIndex()) {
        case 0:
            step->start(direction, k_1, T);
            break;
        case 1:
            qDebug() << "switch gradation->start";
            gradation->start(direction, k_1, k_2, k_3, T1, T2, T3);
            break;
    }
}

void MainWindow::typicalInput_stopByButton() {
    joystick->setOn();
    step->stopByButton();
    ui->pushButton_typicalInput_start->setEnabled(true);
    ui->pushButton_typicalInput_stop->setEnabled(false);

    ui->sdoubleSpinBox_typicalInput_T->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_1->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_2->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_3->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_T1->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_T2->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_T3->         setEnabled(true);
    ui->comboBox_typicalInput_selectedContour-> setEnabled(true);
    ui->comboBox_typicalInput_shape->           setEnabled(true);

    uv_interface.setExperimentTypicalInputFlag(false);
}

void MainWindow::typicalInput_stopByTiemr() {
    joystick->setOn();
    ui->pushButton_typicalInput_start->setEnabled(true);
    ui->pushButton_typicalInput_stop->setEnabled(false);

    ui->sdoubleSpinBox_typicalInput_T->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_1->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_2->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_k_3->        setEnabled(true);
    ui->doubleSpinBox_typicalInput_T1->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_T2->         setEnabled(true);
    ui->doubleSpinBox_typicalInput_T3->         setEnabled(true);
    ui->comboBox_typicalInput_selectedContour-> setEnabled(true);
    ui->comboBox_typicalInput_shape->           setEnabled(true);

    uv_interface.setExperimentTypicalInputFlag(false);
}
