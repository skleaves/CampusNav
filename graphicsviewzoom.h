#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QGraphicsView>

class Graphicsviewzoom : public QObject {
  Q_OBJECT
public:
  Graphicsviewzoom(QGraphicsView* view);
  void gentle_zoom(double factor);
  void set_modifiers(Qt::KeyboardModifiers modifiers);
  void set_zoom_factor_base(double value);

  double m_scalnum = 0.170438;     //缩放系数
  double m_scaldft = 0.170438;     //默认缩放系数0.170438

private:
  QGraphicsView* _view;
  Qt::KeyboardModifiers _modifiers;
  double _zoom_factor_base;
  QPointF target_scene_pos, target_viewport_pos;
  bool eventFilter(QObject* object, QEvent* event);

signals:
  void zoomed(double scale);
};

#endif // GRAPHICSVIEWZOOM_H
