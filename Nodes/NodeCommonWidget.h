#ifndef NODEUIBASE_H
#define NODEUIBASE_H

#include <QWidget>

class Node;

namespace Ui {
class NodeCommonWidget;
}

class NodeCommonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NodeCommonWidget(class Node * node = nullptr);
    ~NodeCommonWidget();

    void OnProcessorSettingsClicked();
    void OnDeleteNodeClicked();
    void OnEnableProcessorCheckboxClicked();
    void OnPeakClicked();
    void OnArrowClicked();

    void SetWidgetVisible(bool visible);
    void SetIsPeakedAt(bool isPeakedAt);
    void ShowLastProcessingTime(int processingTimeMs);

    class Node * GetNode(){ return _node; }

signals:
    void DeleteClicked(class Node * node);
    void PeakClicked(class Node * node);
    void EnableClicked(NodeCommonWidget * processorCommonUI, bool processorEnabled);
    void ArrowClicked(bool isExpanded);

private:
    Ui::NodeCommonWidget *ui;

    class Node * _node;

    bool _isExpanded;
    bool _isPickedAt;;

    void UpdateNodeName(QString name);
};

#endif // NODEUIBASE_H
