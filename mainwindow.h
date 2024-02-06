#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "objviewer.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void changeScale(float scale);

private slots:
    void BeginAutoScale();
    void toStartButtonPressed();
    void toEndButtonPressed();
private:
    float slider_value{0};

    objViewer* viewer;

    QSlider* slider;
    QPushButton* startBtn;
    QPushButton* endBtn;
    Ui::MainWindow *ui;

    bool isIncreasing{true};
    bool isAutoUpdate{false};
    QTimer* timer;
    QTimer* startTimer = nullptr;
    QTimer* endTimer = nullptr;

};
#endif // MAINWINDOW_H
