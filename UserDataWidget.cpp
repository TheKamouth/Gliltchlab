#include "UserDataWidget.h"
#include "ui_UserDataWidget.h"

#include "UserDataManager.h"

#include <QFileSystemModel>

UserDataWidget::UserDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDataWidget)
{
    ui->setupUi(this);

    // D:\5_PROJETS\5_DEV\VirtualScanner\UserData

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath("D:\\5_PROJETS\\5_DEV\\VirtualScanner\\UserData");
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index("D:\\5_PROJETS\\5_DEV\\VirtualScanner\\UserData"));
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
}

UserDataWidget::~UserDataWidget()
{
    delete ui;
}
