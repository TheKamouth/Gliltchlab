#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FlowGraph/ProcessorFlowDockWidget.h"

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
    _glWidget(nullptr),
    _preferencesDialog(parent, &_preferences)
{
    ui->setupUi(this);

    _processorFlowDockWidget = new ProcessorFlowDockWidget(this);
    _processorFlowDockWidget->show();

    _timeControlWidget.hide();
    _timeLineWidget.hide();

    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::PeakNode, this, &MainWindow::OnPeakNode);
    QObject::connect(_processorFlowDockWidget, &ProcessorFlowDockWidget::OutputProcessed, this, &MainWindow::OnOutputProcessed);

    QObject::connect(ui->actionNewFlow, &QAction::triggered ,this, &MainWindow::OnNewFlowGraphFileTriggered);
    QObject::connect(ui->actionSaveFlow, &QAction::triggered ,this, &MainWindow::OnSaveFlowGraphFileTriggered);
    QObject::connect(ui->actionLoadFlow, &QAction::triggered ,this, &MainWindow::OnLoadFlowGraphFileTriggered);

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
    _processorFlowDockWidget->CurrentFlowGraph()->Process();

    _glWidget->SetDisplayedImage(*FlowGraph().Output());
}

void MainWindow::on_actionPreferences_triggered()
{
    _preferencesDialog.GetTmpPref();
    _preferencesDialog.show();
    _preferencesDialog.setWindowTitle( QString("Preferences"));
}

void MainWindow::OnNewFlowGraphFileTriggered()
{
    SaveDialogBeforeChangingCurrentFile();

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

    _processorFlowDockWidget->CurrentFlowGraph()->CreateNewFlowGraphFile(inputFilename);
}

void MainWindow::OnLoadFlowGraphFileTriggered()
{
    SaveDialogBeforeChangingCurrentFile();

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

    _processorFlowDockWidget->CurrentFlowGraph()->LoadFlowGraphFile(inputFilename);
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

    _processorFlowDockWidget->CurrentFlowGraph()->SaveFlowGraphFile(inputFilename);
}

void MainWindow::OnViewTimeControlsTriggered(bool checked)
{
    _timeControlWidget.setVisible(checked);
}

void MainWindow::OnViewTimeLineTriggered(bool checked)
{
    _timeLineWidget.setVisible(checked);
}

void MainWindow::SaveDialogBeforeChangingCurrentFile()
{
    if( _processorFlowDockWidget->CurrentFlowGraph() != nullptr)
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == 0)
        {
            // Save
            OnSaveFlowGraphFileTriggered();
        }
        if(ret == 1)
        {
            // Discard
        }
        else
        {
            // Cancel
            return;
        }
    }
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
