#ifndef GRAPHCONTAINERWIDGET_H
#define GRAPHCONTAINERWIDGET_H

#include "LineGraph.h"

#include <QWidget>
#include <QLayout>

class GraphContainerWidget : public QWidget {
    Q_OBJECT
public:
    // TODO need to have generic graph base class to allow for other graph types
    GraphContainerWidget(QWidget* parent, EoBE::Handle&& graphUpdaterHandle, LineGraph* lineGraph);
    void ResetGraph();

private:
    LineGraph* lineGraph_;
    EoBE::Handle graphUpdaterHandle_;

};

#endif // GRAPHCONTAINERWIDGET_H