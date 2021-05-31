#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FlowGraph/ProcessorFlowDockWidget.h"
#include "Nodes/NodeCommonWidget.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QElapsedTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QCoreApplication>

#include <math.h>       /* cos */
#define PI 3.14159265

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _preferencesDialog(parent, &_preferences),
    _nodePeaked(nullptr),
    _previewWidget(this)
{
    ui->setupUi(this);

    //    QString applicationName = QCoreApplication::applicationName();
    //    qint64 applicationPid = QCoreApplication::applicationPid();
    //    qDebug() << "Application name: " << applicationName << " (" << applicationPid << ")";

    //    _gliltchlabMainProcess.setProgram(QCoreApplication::applicationName());
    //    qDebug() << "member process pid: " << _gliltchlabMainProcess.processId();

    //    QObject::connect(&_readOutputTimer, &QTimer::timeout, this, &MainWindow::OnReadTimerTimout);

    //    _readOutputTimer.setInterval(1000);
    //    _readOutputTimer.start();



    _timeLineWidget = new TimelineWidget();
    addDockWidget(Qt::BottomDockWidgetArea, _timeLineWidget);
    _timeLineWidget->show();

    //_timeControlWidget.hide();

    _glWidget.setParent(&_previewWidget);
    _previewWidget.setWidget(&_glWidget);
    addDockWidget(Qt::RightDockWidgetArea, &_previewWidget);
    _glWidget.setMinimumWidth(300);

    //_previewWidget.setFloating(false);
    //_previewWidget.setWindowFlags(Qt::Tool | Qt::WindowTitleHint);
    //_previewWidget.show();


    // Should be removed when FlowGraphSceneView is done
    //_flowGraphDockWidget = new ProcessorFlowDockWidget(this);
    //_flowGraphDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    //_flowGraphDockWidget->show();
    //addDockWidget(Qt::RightDockWidgetArea, _flowGraphDockWidget);
    //_flowGraphSceneWidget.SetFlowGraph( _flowGraphDockWidget->CurrentFlowGraph());

    _flowGraph = new FlowGraph();
    _flowGraphSceneWidget.SetFlowGraph(_flowGraph);
    setCentralWidget(&_flowGraphSceneWidget);

    //QObject::connect(_flowGraphDockWidget->CurrentFlowGraph(), &FlowGraph::NodeOutputChanged, this, &MainWindow::OnNodeOutputChanged);
    //QObject::connect(_flowGraphDockWidget->CurrentFlowGraph(), &FlowGraph::Processed, this, &MainWindow::OnFlowGraphProcessed);
    //
    //QObject::connect(_flowGraphDockWidget, &ProcessorFlowDockWidget::PeakNode, this, &MainWindow::OnPeakNode);
    //QObject::connect(_flowGraphDockWidget, &ProcessorFlowDockWidget::OutputProcessed, this, &MainWindow::OnOutputProcessed);

    QObject::connect(ui->actionNewFlow, &QAction::triggered ,this, &MainWindow::OnNewFlowGraphFileTriggered);
    QObject::connect(ui->actionSaveFlow, &QAction::triggered ,this, &MainWindow::OnSaveFlowGraphFileTriggered);
    QObject::connect(ui->actionLoadFlow, &QAction::triggered ,this, &MainWindow::OnLoadFlowGraphFileTriggered);

    QObject::connect(ui->actionFlow_widget, &QAction::triggered ,this, &MainWindow::OnViewFlowWidgetTriggered);
    QObject::connect(ui->actionTime_controls, &QAction::triggered ,this, &MainWindow::OnViewTimeControlsTriggered);
    QObject::connect(ui->actionTimeline, &QAction::triggered ,this, &MainWindow::OnViewTimeControlsTriggered);
    QObject::connect(ui->actionDebug_console, &QAction::triggered ,this, &MainWindow::OnShowDebugConsoleTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOutputProcessed()
{
    qDebug() << "Flow processed.";
}

void MainWindow::on_actionPreferences_triggered()
{
    _preferencesDialog.GetTmpPref();
    _preferencesDialog.show();
    _preferencesDialog.setWindowTitle( QString("Preferences"));
}

void MainWindow::OnNewFlowGraphFileTriggered()
{
    /*
    //Dialog box
    QString directory = QString("Open file");
    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();
    QString selectedFilter = QString("Flow graph file (*.fgf)");
    QString inputFilename = QFileDialog::getSaveFileName(this, directory, *filter, selectedFilter);

    if( inputFilename == "" )
    {
        qDebug() << "Error loading file: " << inputFilename << Qt::endl;
        return;
    }
    */

    //_flowGraphDockWidget->CurrentFlowGraph()->CreateNewFlowGraphFile();
    //_flowGraphDockWidget->show();
}

void MainWindow::OnLoadFlowGraphFileTriggered()
{
    //Dialog box
    QString directory = QString("Open file");
    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();
    QString selectedFilter = QString("Flow graph file (*.fgf)");
    QString inputFilename = QFileDialog::getOpenFileName(this, directory, *filter, selectedFilter);

    if( inputFilename == "" )
    {
        qDebug() << "Error loading file: " << inputFilename << Qt::endl;
        return;
    }

    //_flowGraphDockWidget->CurrentFlowGraph()->LoadFlowGraphFile(inputFilename);
    //_flowGraphDockWidget->show();
}

void MainWindow::OnSaveFlowGraphFileTriggered()
{
    //Dialog box
    QString directory = QString("Open file");
    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();
    QString selectedFilter = QString("Flow graph file (*.fgf)");
    QString inputFilename = QFileDialog::getSaveFileName(this, directory, *filter, selectedFilter);

    if( inputFilename == "" )
    {
        qDebug() << "Error loading file: " << inputFilename << Qt::endl;
        return;
    }

    //_flowGraphDockWidget->CurrentFlowGraph()->SaveFlowGraphFile(inputFilename);
}

void MainWindow::OnReadTimerTimout()
{
    QString outputStdOut = _gliltchlabMainProcess.readAllStandardOutput();
    QString outputStdErr = _gliltchlabMainProcess.readAllStandardError();

    ui->actionDebug_console->text().append(outputStdOut);
    ui->actionDebug_console->text().append(outputStdErr);

}

void MainWindow::OnViewFlowWidgetTriggered(bool checked)
{
    //_flowGraphDockWidget->setVisible(checked);
}


void MainWindow::OnViewTimeControlsTriggered(bool checked)
{
    _timeControlWidget.setVisible(checked);
}

void MainWindow::OnViewTimeLineTriggered(bool checked)
{
    _timeLineWidget->setVisible(checked);
}

void MainWindow::OnShowDebugConsoleTriggered(bool checked)
{
    ui->actionDebug_console->setVisible(checked);
}

void MainWindow::OnPeakNode(INode *node)
{
    if (node == _nodePeaked)
    {
        // node already peaked at
        return;
    }

    /*
    _glWidget.SetDisplayedImage(node->MainOutput());
    _nodePeaked = node;

    FlowGraph * flowGraph = _flowGraphDockWidget->CurrentFlowGraph();
    for (int i = 0 ; i < flowGraph->NodeCount() ; i++)
    {
        if (i != _nodePeaked->Position())
        {
            flowGraph->GetNode(i)->CommonWidget()->SetIsPeakedAt(false);
        }
    }

    _nodePeaked->CommonWidget()->SetIsPeakedAt(true);
    */
}

void MainWindow::OnNodeOutputChanged(INode *node)
{
    /*
    if(node == _nodePeaked)
    {
        _glWidget.SetDisplayedImage(node->Output());
    }
    */
}

void MainWindow::OnFlowGraphProcessed()
{
    if ( _nodePeaked == nullptr)
    {
        return;
    }

    //_glWidget.SetDisplayedImage(_nodePeaked->Output());
}
