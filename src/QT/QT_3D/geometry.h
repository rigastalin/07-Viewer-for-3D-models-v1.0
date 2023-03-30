#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#include "../../parse.h"
}
#endif

class GeometryEngine : protected QOpenGLFunctions {
    struct VertexData {
        QVector3D position;
    };

 public:
    GeometryEngine();
    virtual ~GeometryEngine();

    QString filepath, vCount, fCount;

    double newScale = 1, newMoveX = 0, newMoveY = 0, newMoveZ = 0,
    newRotationX = 0, newRotationY = 0, newRotationZ = 0, lineWidth = 1.0;

    unsigned int vertexCount = 0, facetsCount = 0;

    int flagDot = 0, flagParallel = 0, flagVertexOn = 0,
    f_ver = 0, f_sphere = 0, size_of_ver = 2;

    void movingX(double val);
    void movingY(double val);
    void movingZ(double val);

    void rotatingX(double val);
    void rotatingY(double val);
    void rotatingZ(double val);

    void drawing(QOpenGLShaderProgram *m_program);
    void binding();
    void parsingObj();
    void scaling(double val);
    void drawing_vertex(QOpenGLShaderProgram *m_program);

    int countF();
    int countEDGES();

 private:
    QOpenGLBuffer arrayBuff;
    QOpenGLBuffer indexBuff;

    GLsizei lines;
};

#endif  // GEOMETRY_H
