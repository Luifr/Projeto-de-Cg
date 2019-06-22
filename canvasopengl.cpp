#include "canvasopengl.h"

CanvasOpenGL::CanvasOpenGL()
{

}

void CanvasOpenGL::setParameters()
{

}

void CanvasOpenGL::resetParameters()
{

}

void CanvasOpenGL::toggleProjection()
{

}

void CanvasOpenGL::perspectiveGL()
{

}

void CanvasOpenGL::LookAt()
{

}

void CanvasOpenGL::drawLines(vector<QVector3D> *vertices)
{
    (void) vertices;
}

void CanvasOpenGL::fillPoligon(PolygonS* polygon)
{
    (void) polygon;
}

GLdouble CanvasOpenGL::euclidean(QVector3D p1, QVector3D p2)
{
    GLdouble result = GL_DOUBLE;
    (void) p1;
    (void) p2;
    return result;
}

void CanvasOpenGL::reset()
{

}

void CanvasOpenGL::initializeGL()
{

}
void CanvasOpenGL::resizeGL(GLint w, GLint h) {
    (void) w;
    (void) h;
}
void CanvasOpenGL::paintGL() {

}

void CanvasOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    (void) event;
}

void CanvasOpenGL::mousePressEvent(QMouseEvent *event)
{
    (void) event;
}

void  CanvasOpenGL::wheelEvent (QWheelEvent * event)
{
    (void) event;
}
