#ifndef RGBSHIFTFILTERNODE_H
#define RGBSHIFTFILTERNODE_H

#include <QWidget>

namespace Ui {
class RgbShiftFilterNode;
}

class RgbShiftFilterNode : public QWidget
{
    Q_OBJECT

public:
    explicit RgbShiftFilterNode(QWidget *parent = nullptr);
    ~RgbShiftFilterNode();

private:
    Ui::RgbShiftFilterNode *ui;
};

#endif // RGBSHIFTFILTERNODE_H
