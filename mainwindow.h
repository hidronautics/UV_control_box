#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSettings>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QDebug>
#include <QShortcut>
#include <QApplication>
#include <QThread>
#include <QTimer>


#include "UV/iuserinterfacedata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateUi_fromROV(); //обновляет на экране данные, пришедшие с аппарата
    void updateUi_fromControl(); //обновляет на экране данные, пришедшие с джостика
    void updateUi_cSModeChange(e_CSMode mode); //синхронизация radio_button и

private:
    Ui::MainWindow *ui;

    IUserInterfaceData uv_interface;

};
#endif // MAINWINDOW_H
