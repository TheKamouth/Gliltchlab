#include "PixelSortFilterNode.h"
#include "ui_PixelSortFilterNode.h"

PixelSortFilterNode::PixelSortFilterNode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PixelSortFilterNode)
{
    ui->setupUi(this);
}

PixelSortFilterNode::~PixelSortFilterNode()
{
    delete ui;
}
