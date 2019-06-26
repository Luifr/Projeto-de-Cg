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
#include <QDebug>

// C Libs
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <edge.h>
#include <list>
#include <iostream>
#include <fstream>

// Custom Structs
#include "structs.h"


using namespace std;

typedef struct vecc{
    float x;
    float y;
    float z;
}Vector;

typedef struct pointt{
    float x;
    float y;
    float z;
}Point;

typedef struct planee{
    float Xcoef;
    float Ycoef;
    float Zcoef;
    float independentTerm;
}Plane;

//angle between normal and axis
typedef struct ang{
    float x_axis;
    float y_axis;
}Angles;

class CanvasOpenGL : public QOpenGLWidget
{
public:
    CanvasOpenGL(QWidget *parent);
    ~CanvasOpenGL();
    void clearScreen();

    void doScanLine(std::map<int,std::list<Edge>> ET);
    void scanLine(Point points[3]);
    vector<Point> pointsToPaint;

private:

    float heightmap[256][256];
    int r,g,b;
    bool draw;
    int radius;

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
    void readInputFile();
    void printHeightMap();

    // OBSERVER
    QVector3D *up, *eye, *center;
    GLfloat xRot, yRot, zRot;
    QPoint first, last;
    void LookAt();

    // polygon
    PolygonS* currentPolygon;
    vector<PolygonS*> polygons;
    std::map<int,std::list<Edge>> ET;
    std::list<Edge> AET;
    vector<QPoint> vertices;

    // SCANLINE
    void drawLines(vector<QVector3D> *vertices);
    void fillPoligon(PolygonS* polygon);



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
