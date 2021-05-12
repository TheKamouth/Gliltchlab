#ifndef NODEUIBASE_H
#define NODEUIBASE_H

#include <QWidget>

class Node;

namespace Ui {
class NodeUiBase;
}

class NodeCommonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NodeCommonWidget(Node * node = nullptr);
    ~NodeCommonWidget();

    void OnProcessorSettingsClicked();
    void OnDeleteNodeClicked();
    void OnEnableProcessorCheckboxClicked();
    void OnPeakClicked();
    void OnArrowClicked();

    void SetWidgetVisible(bool visible);
    void SetPeakedStatus(bool isPeakedAt);

    Node * GetNode(){ return _node; }

signals:
    void DeleteClicked(Node * node);
    void PeakClicked(Node * node);
    void EnableClicked(NodeCommonWidget * processorCommonUI, bool processorEnabled);
    void ArrowClicked(bool isExpanded);

private:
    Ui::NodeUiBase *ui;

    Node * _node;

    bool _isExpanded;
    bool _isPickedAt;;

    void UpdateNodeName(QString name);
};

#endif // NODEUIBASE_H
