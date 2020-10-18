#ifndef UNIVERSEWIDGET_H
#define UNIVERSEWIDGET_H

#include "Shape.h"
#include "Universe.h"

#include <QWidget>
#include <QTimer>

class UniverseWidget final : public QWidget, public UniverseObserver {
    Q_OBJECT
public:
    explicit UniverseWidget(QWidget* parent);
    virtual ~UniverseWidget();

    virtual void SuggestFocus(const Point& focus) override;
    virtual void SuggestUpdate() override { updateToRender_ = true; }

    void SetUniverse(std::shared_ptr<Universe> universe);
    void SetFpsTarget(double fps);

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* event) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void keyPressEvent(QKeyEvent* event) override final;

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent* event) override final;
private:
    QTimer renderThread_;
    bool updateToRender_ = false;

    // TODO update to using a matrix based transform
    qreal transformX_ = 0.0;
    qreal transformY_ = 0.0;
    qreal transformScale_ = 1.0;
    qreal dragX_ = 0.0;
    qreal dragY_ = 0.0;
    bool dragging_ = false;

    std::shared_ptr<Universe> universe_;

    Point TransformLocalToSimCoords(const Point& local);
    Point TransformSimToLocalCoords(const Point& sim);
};

#endif // UNIVERSEWIDGET_H