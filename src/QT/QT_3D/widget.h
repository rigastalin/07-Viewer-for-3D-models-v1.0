#ifndef WIDGET_H
#define WIDGET_H

#include "geometry.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>

#ifdef __cplusplus
extern "C" {
#include "../../parse.h"
#include "../../afins.h"
}
#endif

class GeometryEngine;

class widget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

 public:
    using QOpenGLWidget::QOpenGLWidget;
    ~widget();

    QColor *color_ver = new QColor(12, 26, 114, 0);
    QColor *color_back = new QColor(247, 224, 57, 0);
    QColor *color_face = new QColor(65, 83, 197, 255);

    QOpenGLShaderProgram m_program;

    void initColors();
    void initColorsFace();
    void initColorsVer();
    void updating();
    void drawing(QOpenGLShaderProgram *program);

    void resizeGL(int w, int h) override;

    GeometryEngine *geometries = nullptr;
    QString filepath = "", vCount, fCount;
    double scale = 0.99, P = 0;
    int flagParallel = 0, flagDot, index = 0, f_ver = 0, f_sphere = 0, lineWidth = 0, size_of_ver = 0, loadflag = 0;

    void initializeGL() override;

 protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void paintGL() override;

    void initShades();

 private:
    QMatrix4x4 m_projectionMatrix;

    QBasicTimer timer;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

};
#endif  // WIDGET_H
