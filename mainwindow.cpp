#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    connect(action_create_vehicle, SIGNAL(triggered()), this, SLOT(createVehicle()));
//    connect(radioButton_ConnectionConfig, SIGNAL(clicked()), this, SLOT(configConnectionClick()));


    connect(ui->radioButton_cSMode_auto,            SIGNAL(clicked()), this, SLOT(cSModeChange_auto()));
    connect(ui->radioButton_cSMode_identification1, SIGNAL(clicked()), this, SLOT(cSModeChange_identification1()));
    connect(ui->radioButton_cSMode_identification2, SIGNAL(clicked()), this, SLOT(cSModeChange_identification2()));
    connect(ui->radioButton_cSMode_handle,          SIGNAL(clicked()), this, SLOT(cSModeChange_handle()));
    connect(ui->checkBox_cSMode_thruster, SIGNAL(toggled(bool)), this, SLOT(cSModeChange_thruster(bool)));
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
    ui->label_control_pitch     ->setText(QString::number(control.roll,     'f', 2));
}

void MainWindow::updateUi_fromROV() {
    ImuData imuData = uv_interface.getImuData();

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


//void MainWindow::updateUi_cSModeChange() {
//
//    e_CSMode mode;
//    mode = e_CSMode::MODE_HANDLE;
//
//    mode == e_CSMode::MODE_AUTO ? ui->radioButton_cSMode_auto->setChecked(true) :
//    ui->radioButton_cSMode_auto->setChecked(false);
//
//    mode == e_CSMode::MODE_IDENTIFICATION_1 ? ui->radioButton_cSMode_identification1->setChecked(true) :
//    ui->radioButton_cSMode_identification1->setChecked(false);
//
//    mode == e_CSMode::MODE_IDENTIFICATION_2 ? ui->radioButton_cSMode_identification2->setChecked(true) :
//    ui->radioButton_cSMode_identification2->setChecked(false);
//
//    mode == e_CSMode::MODE_HANDLE ? ui->radioButton_cSMode_handle->setChecked(true) :
//    ui->radioButton_cSMode_handle->setChecked(false);
//}


