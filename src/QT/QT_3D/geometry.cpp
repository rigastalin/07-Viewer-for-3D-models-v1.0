#include "geometry.h"

#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QVector>

#ifdef __cplusplus
extern "C" {
#include "../../parse.h"
#include "../../afins.h"
}
#endif

struct VertexData {
    QVector3D position;
};

data_struct data;

GeometryEngine::GeometryEngine()
    : indexBuff(QOpenGLBuffer::IndexBuffer) {
    initializeOpenGLFunctions();
    arrayBuff.create();
    indexBuff.create();
}

GeometryEngine::~GeometryEngine() {
    arrayBuff.destroy();
    indexBuff.destroy();
}

void GeometryEngine::parsingObj() {
    QFile file(filepath);
    if (!file.exists()) {
        return;
    } else {
        data.count_V = 0;
        data.count_F = 0;
        data.count_EDGES = 0;
        QFile input(QString(this->filepath));
        input.open(QIODevice::ReadOnly);
        int fd = input.handle();
        FILE* fl = fdopen(fd, "rb");
        counters_v(&data, fl);
        counters_f(&data, fl);
        matrixFill(&data, fl);
        polygonFill(&data, fl);

        vertexCount = data.count_F;
        facetsCount = data.count_EDGES;
        vCount = QString::number(vertexCount);
        fCount = QString::number(facetsCount);
        input.close();

        binding();

        QVector<GLuint> indexes;
        for (int i = 0; i < data.count_F; i++) {
            for (int j = 0; j < data.polygons[i].numofone; j++) {
                indexes.push_back(data.polygons[i].vertexes[j]);
            }
        }

        lines = indexes.length();
        indexBuff.bind();
        indexBuff.allocate(indexes.data(), sizeof(GLuint) * indexes.length());
    }
}

void GeometryEngine::binding() {
    QVector<VertexData> vertexes;
    for (int j = 0; j < data.count_V; j++) {
        VertexData temp = {(QVector3D(data.matrix_3d[j].matrix[0][0],
                       data.matrix_3d[j].matrix[1][0], data.matrix_3d[j].matrix[2][0]))};
            vertexes.push_back(temp);
    }
    arrayBuff.bind();
    arrayBuff.allocate(vertexes.data(), sizeof(VertexData) * vertexes.length());
}


void GeometryEngine::drawing(QOpenGLShaderProgram *m_program) {
    if (f_ver) {
        if (f_sphere) {
            glDisable(GL_POINT_SIZE);
            glEnable(GL_POINT_SMOOTH);
        } else {
            glDisable(GL_POINT_SMOOTH);
            glEnable(GL_POINT_SIZE);
        }
    }
    glPointSize(size_of_ver);

    glLineWidth(lineWidth);

    binding();
    arrayBuff.bind();
    indexBuff.bind();

    quintptr offset = 0;

    int vertexLoc = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLoc);
    m_program->setAttributeBuffer(vertexLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLoc = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLoc);
    m_program->setAttributeBuffer(texcoordLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_LINES, lines, GL_UNSIGNED_INT, nullptr);
}

void GeometryEngine::drawing_vertex(QOpenGLShaderProgram *m_program) {
    if (f_ver) {
        if (f_sphere) {
            glDisable(GL_POINT_SIZE);
            glEnable(GL_POINT_SMOOTH);
            glPointSize(size_of_ver);
        } else {
            glDisable(GL_POINT_SMOOTH);
            glEnable(GL_POINT_SIZE);
            glPointSize(size_of_ver);
        }
    } else {
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_POINT_SIZE);
        glPointSize(1);
    }

    glLineWidth(lineWidth);

    binding();
    arrayBuff.bind();
    indexBuff.bind();

    quintptr offset = 0;

    int vertexLoc = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLoc);
    m_program->setAttributeBuffer(vertexLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLoc = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLoc);
    m_program->setAttributeBuffer(texcoordLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_POINTS, lines, GL_UNSIGNED_INT, nullptr);
}

void GeometryEngine::scaling(double val) {
    if (val != 0.0) {
        scale(&data, val / newScale);
        newScale = val;
    }
}

void GeometryEngine::movingX(double val) {
    moveX(&data, val - newMoveX);
    newMoveX = val;
}

void GeometryEngine::movingY(double val) {
    moveY(&data, val - newMoveY);
    newMoveY = val;
}

void GeometryEngine::movingZ(double val) {
    moveZ(&data, val - newMoveZ);
    newMoveZ = val;
}

void GeometryEngine::rotatingX(double val) {
    rotationX(&data, val - newRotationX);
    newRotationX = val;
}

void GeometryEngine::rotatingY(double val) {
    rotationY(&data, val - newRotationY);
    newRotationY = val;
}

void GeometryEngine::rotatingZ(double val) {
    rotationZ(&data, val - newRotationZ);
    newRotationZ = val;
}
