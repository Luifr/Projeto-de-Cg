#ifndef CANVASOPENGL_H
#define CANVASOPENGL_H

// Qt Libs
#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QVector3D>
#include <QMatrix4x4>

// C Libs
#include <vector>
#include <stdlib.h>
#include <math.h>

// Custom Structs
#include "structs.h"


using namespace std;

class CanvasOpenGL : public QOpenGLWidget
{
public:
    CanvasOpenGL();

private:
    // VIEWING
    GLint height, width;
    GLdouble
        hMin,   hMax,
        vMin,   vMax,
        nearV,   farV,
        aspect, fovY;
    void setParameters();
    void resetParameters();
    void toggleProjection ();
    void perspectiveGL();

    // OBSERVER
    QVector3D *up, *eye, *center;
    GLfloat xRot, yRot, zRot;
    QPoint lastPos;
    void LookAt();

    // polygon
    PolygonS* currentPolygon;
    vector<PolygonS*> polygons;

    // SCANLINE
    void drawLines(vector<QVector3D> *vertices);
    void fillPoligon(PolygonS* polygon);

    // ADT
    vector<Edge*> edgeTable;
    vector<GLint> activeEdgeList;

    // HELPERS
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble euclidean (QVector3D, QVector3D);
    void reset();

protected:
    // OpenGL
    void initializeGL();
    void resizeGL(GLint w, GLint h);
    void paintGL();

    // Qt Events
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent * event);
};

#endif // CANVASOPENGL_H
