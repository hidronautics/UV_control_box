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

    ui->label_control_march->setText(QString::number(control.march, 'f', 2));
    ui->label_control_lag->setText(QString::number(control.lag, 'f', 2));
    ui->label_control_depth->setText(QString::number(control.depth, 'f', 2));
    ui->label_control_yaw->setText(QString::number(control.yaw, 'f', 2));
    ui->label_control_roll->setText(QString::number(control.roll, 'f', 2));
}

void MainWindow::updateUi_fromROV() {

}

