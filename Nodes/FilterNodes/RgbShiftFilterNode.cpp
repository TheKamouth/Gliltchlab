#include "RgbShiftFilterNode.h"
#include "ui_RgbShiftFilterNode.h"

RgbShiftFilterNode::RgbShiftFilterNode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RgbShiftFilterNode)
{
    ui->setupUi(this);
}

RgbShiftFilterNode::~RgbShiftFilterNode()
{
    delete ui;
}
