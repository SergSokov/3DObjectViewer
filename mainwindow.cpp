#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , slider_value{0}
{
    ui->setupUi(this);

    ui->slider->setMaximum(300);
    ui->slider->setMinimum(0);
    ui->slider->setValue(0);
    ui->slider->setInvertedAppearance(true);

    QToolButton* helpButton = new QToolButton();
    helpButton->setText("Помощь");

    connect(helpButton, &QToolButton::pressed, [this]{
       QMessageBox::information(this, "Справка", "Кнопка \"В начало\" - вернуть анимацию к началу\n"
                                                 "Кнопка \"В конец\" - перевести анимацию в конец\n"
                                                 "Кнопка \"Повторять\" - циклически повторять анимацию\n"
                                                 "Зажатый ЛКМ - перемещать модель\n"
                                                 "Зажатый ПКМ - вращать модель\n"
                                                 "Нажать СКМ - вернуть модель в начальную позицию");
    });

    ui->toolBar->addWidget(helpButton);
    timer = new QTimer(this);

    connect(ui->slider, &QSlider::valueChanged, [this](int value){
        emit changeScale((float)value);
    });


    startTimer = new QTimer(this);
    startTimer->setInterval(15);

    connect(startTimer, &QTimer::timeout, [this]{

        int value = ui->slider->value();
        int plusMinus = -1;

        if(value == ui->slider->minimum())
        {
            startTimer->stop();
        }
        else
            ui->slider->setValue(value += plusMinus);
    });

    endTimer = new QTimer(this);
    endTimer->setInterval(15);

    connect(endTimer, &QTimer::timeout, [this]{

        int value = ui->slider->value();
        int plusMinus = 1;

        if(value == ui->slider->maximum())
        {
            endTimer->stop();
        }
        else
            ui->slider->setValue(value += plusMinus);
    });

    viewer = new objViewer();

    if(!viewer)
    {
        QMessageBox::critical(this,"Ошибка","Провал!");
        return;
    }
    ui->horizontalLayout->addWidget(viewer);

    viewer->installEventFilter(this);

    connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::toStartButtonPressed);
    connect(ui->endButton, &QPushButton::pressed, this, &MainWindow::toEndButtonPressed);
    connect(this, &MainWindow::changeScale, viewer, &objViewer::changeObjectForm);

    connect(ui->repeatButton, &QPushButton::pressed, this, &MainWindow::BeginAutoScale);
}

void MainWindow::toStartButtonPressed()
{
    if(ui->slider->value() == ui->slider->minimum())
        return;

    if(startTimer->isActive())
    {
        startTimer->stop();
        return;
    }
    if(endTimer->isActive())
        endTimer->stop();

    startTimer->start();
}

void MainWindow::toEndButtonPressed()
{
    if(ui->slider->value() == ui->slider->maximum())
        return;

    if(endTimer->isActive())
    {
        endTimer->stop();
        return;
    }
    if(startTimer->isActive())
        startTimer->stop();

    endTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BeginAutoScale()
{
    if(startTimer->isActive())
        startTimer->stop();
    if(endTimer->isActive())
        endTimer->stop();

    if(!isAutoUpdate)
    {
        ui->startButton->setEnabled(false);
        ui->endButton->setEnabled(false);

        timer->setInterval(15);
        connect(timer, &QTimer::timeout, [this]{

            int value = ui->slider->value();
            int plusMinus = 1;

            if(value == ui->slider->maximum())
                isIncreasing = false;

            if(value == ui->slider->minimum())
                isIncreasing = true;

            if(!isIncreasing)
                plusMinus *= -1;

            ui->slider->setValue(value += plusMinus);
        });
        isAutoUpdate = true;
        timer->start();
    }
    else
    {
        ui->startButton->setEnabled(true);
        ui->endButton->setEnabled(true);

        if(timer->isActive())
            timer->stop();
        qDebug() << "Stoping timer";
        isAutoUpdate = false;
    }
}
