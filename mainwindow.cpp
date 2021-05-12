#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ImageProcessors/ContrastProcessor.h"
#include "ImageProcessors/Scanner.h"

#include "ProcessorFlowDockWidget.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QElapsedTimer>
#include <QDateTime>

#include <math.h>       /* cos */
#define PI 3.14159265

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _preferencesDialog(parent, &_preferences),
    _currentPeakWidget(nullptr)
{
    ui->setupUi(this);

    _processorFlowDockWidget = new ProcessorFlowDockWidget(this);
    _processorFlowDockWidget->show();

    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::PeakNode, this, &MainWindow::OnPeakNode);
    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::OutputProcessed, this, &MainWindow::OnOutputProcessed);

    //_glWidget = new OpenGLWidget(this);

    //this->setCentralWidget(_glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOutputProcessed(QImage * output)
{
    _glWidget->SetDisplayedImage(*output);
}

void MainWindow::on_actionPlay_triggered()
{
    //_scanner.ScanOneDrawCall();

    _processorFlow.PlayFlow();

    _glWidget->SetDisplayedImage(*_processorFlow.Output());
}

void MainWindow::on_actionPreferences_triggered()
{
    _preferencesDialog.GetTmpPref();
    _preferencesDialog.show();
    _preferencesDialog.setWindowTitle( QString("Preferences"));
}

void MainWindow::OnPeakNode(Node *node)
{
    if( _currentPeakWidget != nullptr)
    {
        delete _currentPeakWidget;
    }

    // hm, this does not feel right
    _glWidget = dynamic_cast<OpenGLWidget*>(node->InstantiatePeakWidget());

    //_glWidget->setParent(this);
    setCentralWidget(_glWidget);
    //_glWidget->setVisible(true);
}
