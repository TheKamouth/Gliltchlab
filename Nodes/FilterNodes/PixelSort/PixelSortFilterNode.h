#ifndef PIXELSORTFILTERNODE_H
#define PIXELSORTFILTERNODE_H

#include <QWidget>

namespace Ui {
class PixelSortFilterNode;
}

class PixelSortFilterNode : public QWidget
{
    Q_OBJECT

public:
    explicit PixelSortFilterNode(QWidget *parent = nullptr);
    ~PixelSortFilterNode();

private:
    Ui::PixelSortFilterNode *ui;
};

#endif // PIXELSORTFILTERNODE_H
