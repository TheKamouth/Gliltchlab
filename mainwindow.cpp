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

#include <math.h>       /* cos */
#define PI 3.14159265

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _preferencesDialog(parent, &_preferences),
    _nodePeaked(nullptr),
    _peakWidget(nullptr)
{
    ui->setupUi(this);

    _flowGraphDockWidget = new ProcessorFlowDockWidget(this);
    _flowGraphDockWidget->hide();
    addDockWidget(Qt::RightDockWidgetArea, _flowGraphDockWidget);

    _timeLineWidget = new TimelineWidget(this);
    _timeLineWidget->hide();
    addDockWidget(Qt::BottomDockWidgetArea, _timeLineWidget);

    _timeControlWidget.hide();

    QObject::connect(_flowGraphDockWidget, &ProcessorFlowDockWidget::PeakNode, this, &MainWindow::OnPeakNode);
    QObject::connect(_flowGraphDockWidget, &ProcessorFlowDockWidget::OutputProcessed, this, &MainWindow::OnOutputProcessed);

    QObject::connect(ui->actionNewFlow, &QAction::triggered ,this, &MainWindow::OnNewFlowGraphFileTriggered);
    QObject::connect(ui->actionSaveFlow, &QAction::triggered ,this, &MainWindow::OnSaveFlowGraphFileTriggered);
    QObject::connect(ui->actionLoadFlow, &QAction::triggered ,this, &MainWindow::OnLoadFlowGraphFileTriggered);

    QObject::connect(ui->actionFlow_widget, &QAction::triggered ,this, &MainWindow::OnViewFlowWidgetTriggered);
    QObject::connect(ui->actionTime_controls, &QAction::triggered ,this, &MainWindow::OnViewTimeControlsTriggered);
    QObject::connect(ui->actionTimeline, &QAction::triggered ,this, &MainWindow::OnViewTimeControlsTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOutputProcessed()
{
    qDebug() << "Flow processed.";
}

void MainWindow::on_actionPlay_triggered()
{
    //_processorFlow.PlayFlow();
    _flowGraphDockWidget->CurrentFlowGraph()->Process();

    //_glWidget->SetDisplayedImage(*FlowGraph().Output());
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

    _flowGraphDockWidget->CurrentFlowGraph()->CreateNewFlowGraphFile();
    _flowGraphDockWidget->show();
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

    _flowGraphDockWidget->CurrentFlowGraph()->LoadFlowGraphFile(inputFilename);
    _flowGraphDockWidget->show();
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

    _flowGraphDockWidget->CurrentFlowGraph()->SaveFlowGraphFile(inputFilename);
}

void MainWindow::OnViewFlowWidgetTriggered(bool checked)
{
    _flowGraphDockWidget->setVisible(checked);
}


void MainWindow::OnViewTimeControlsTriggered(bool checked)
{
    _timeControlWidget.setVisible(checked);
}

void MainWindow::OnViewTimeLineTriggered(bool checked)
{
    _timeLineWidget->setVisible(checked);
}

void MainWindow::OnPeakNode(Node *node)
{
    if (node == _nodePeaked)
    {
        // node already peaked at
        return;
    }

    ViewInfo * viewInfo = nullptr;
    OpenGLWidget * glWidget = nullptr;

    if (_nodePeaked != nullptr)
    {
        // Currently peaking a node

        // hm, this does not feel right
        glWidget = dynamic_cast<OpenGLWidget*>(_peakWidget);
        if (glWidget != nullptr)
        {
            // _peakWidget is already an OpenGLWidget
            // Copy previous viewInfo
            viewInfo = new ViewInfo(*glWidget->GetViewInfo());
        }

        // Delete its widget
        _nodePeaked->ReleasePeakWidget();
    }

    _nodePeaked = node;
    _peakWidget = _nodePeaked->InstantiatePeakWidget();
    glWidget = dynamic_cast<OpenGLWidget*>(_peakWidget);

    FlowGraph * flowGraph = _flowGraphDockWidget->CurrentFlowGraph();
    for (int i = 0 ; i < flowGraph->NodeCount() ; i++)
    {
        if (i != _nodePeaked->Position())
        {
            flowGraph->GetNode(i)->CommonWidget()->SetIsPeakedAt(false);
        }
    }

    _nodePeaked->CommonWidget()->SetIsPeakedAt(true);

    if(viewInfo != nullptr)
    {
        glWidget->SetViewInfo(viewInfo);
    }

    setCentralWidget(_peakWidget);
}
