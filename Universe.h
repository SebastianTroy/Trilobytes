#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "QuadTree.h"
#include "EnergyPool.h"
#include "LineGraph.h"
#include "FeedDispenser.h"

#include <QScrollArea>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include <iomanip>
#include <math.h>

class Universe : public QWidget {
    Q_OBJECT
public:
    Universe(QWidget *parent);

    void SetupGraphs(LineGraph& graph);

signals:
    void OnFoodEnergySampled(double energy);
    void OnSwimmerEnergySampled(double energy);
    void OnGraphResetRequested();

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* /*event*/) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void keyPressEvent(QKeyEvent* event) override;

    virtual void paintEvent(QPaintEvent* event) override final;

private:
    double simX_ = 0.0;
    double simY_ = 0.0;
    double simScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    bool spawnFood_ = true;
    bool pauseSim_ = false;
    bool respawn_ = true;

    EnergyPool& energy_;
    QuadTree rootNode_;
    std::vector<FeedDispenser> feedDispensers_;

    void Thread();
};

#endif // UNIVERSE_H
