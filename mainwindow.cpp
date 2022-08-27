#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::updateUi_cSModeChange(e_CSMode mode) {
    if (mode == e_CSMode::MODE_AUTO){
        ui->radioButton_cSMode_auto ->setChecked(true);
    }



}
