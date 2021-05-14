#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    _preferencesDialog(parent, &_preferences)
{
    ui->setupUi(this);

    _processorFlowDockWidget = new ProcessorFlowDockWidget(this);
    _processorFlowDockWidget->show();

    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::PeakNode, this, &MainWindow::OnPeakNode);
    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::OutputProcessed, this, &MainWindow::OnOutputProcessed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOutputProcessed(QImage * output)
{
    qDebug() << "Flow processed.";
}

void MainWindow::on_actionPlay_triggered()
{
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
    ViewInfo * viewInfo = nullptr;
    if(_glWidget != nullptr)
    {
        viewInfo = _glWidget->GetViewInfo();
    }

    // hm, this does not feel right
    _glWidget = dynamic_cast<OpenGLWidget*>(node->InstantiatePeakWidget());

    if(viewInfo != nullptr)
    {
        _glWidget->SetViewInfo(viewInfo);
    }
    setCentralWidget(_glWidget);
}
