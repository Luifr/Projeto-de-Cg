#include "canvasopengl.h"
#include <QDebug>
#include <map>
#include <edge.h>
#include <math.h>

CanvasOpenGL::CanvasOpenGL(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug("esd");
    this->draw = false;
    this->radius = 12;
    this->r = 255;
    this->g = 255;
    this->b = 255;
}

CanvasOpenGL::~CanvasOpenGL(){

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
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
    QPen myPen(1); // 1 px

    QColor color(this->r, this->g, this->b); // WHITE
    myPen.setColor(color);

    painter.setPen(myPen);

    QPoint last; // draw edge
    for (vector<QPoint>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
        painter.drawPoint(*i);
        if(last.isNull() == false){
            painter.drawLine(*i,last);
        }
        last = *i;
    }

    if(this->draw){ // if polygon is finished, paint it
        int scanLine = this->ET.begin()->first;
        int yMax=0;
        for (std::map<int, list<Edge>>::iterator it = this->ET.begin();it != this->ET.end();it++) {
            for (list<Edge>::iterator iterator = it->second.begin(), end = it->second.end(); iterator != end; ++iterator) {
                if(iterator->yMax > yMax){
                    yMax = iterator->yMax;
                }
            }
        }

        for( ; scanLine < yMax ; scanLine++){ // scanlines
            if(this->ET.find(scanLine) != this->ET.end()){
                this->AET.splice(this->AET.end(),this->ET.find(scanLine)->second);
            }
            for (std::list<Edge>::iterator iterator = this->AET.begin(), end = this->AET.end(); iterator != end; ++iterator) {
                if(iterator->yMax == scanLine){
                    this->AET.remove(*iterator);
                }
                else{
                    iterator->x += iterator->xInc;
                }
            }
            this->AET.sort([](Edge lhs, Edge rhs) {return lhs.x< rhs.x;});
            float first = 0;
            for (std::list<Edge>::const_iterator iterator = this->AET.begin(), end = this->AET.end(); iterator != end; ++iterator) {
                if(int(first) == 0) {
                    first = iterator->x;
                }
                else {
                    for(int i = int(ceil(first)) ; i < floor(iterator->x) ; i++){ // fill the scanline
                        painter.drawPoint(i,scanLine);
                    }
                    first = 0;
                }
            }
        }
    }

}

void CanvasOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    (void) event;
}

void CanvasOpenGL::mousePressEvent(QMouseEvent *event) {
    qDebug("asd");
    qDebug(draw ? "t" : "f");
    if(draw){ // if a polygon is drawn, dont add more points
        return;
    }

    QPoint pos = event->pos();

    if(this->first.isNull()){
        this->first = pos;
    }
    else{
        if(std::abs(pos.x() - this->first.x()) < this->radius && std::abs(pos.y() - this->first.y()) < this->radius){
            pos = this->first;
            this->draw = true;
        }
    }

    this->vertices.push_back(pos);

    if(!this->last.isNull() && pos.y() != this->last.y()){ // se esse nao for o primeiro ponto, e nao for uma reta horizontal
        int yMin = min(this->last.y(),pos.y());
        Edge e(pos,this->last);

        std::map<int, list<Edge>>::iterator it = this->ET.find(yMin); // adiciona na ET
        if(it == this->ET.end()){
            list<Edge> l;
            l.push_front(e);
            this->ET[yMin] = l;
        }
        else{
            it->second.push_front(e);
        }

    }

    this->last = pos;


    this->update();
}

void  CanvasOpenGL::wheelEvent (QWheelEvent * event)
{
    (void) event;
}

void CanvasOpenGL::clearScreen() {
    this->ET.clear();
    this->AET.clear();

    this->draw = false;

    this->first.setX(0);
    this->first.setY(0);

    this->last.setX(0);
    this->last.setY(0);

    if(!this->vertices.empty()) {
        this->vertices.clear();
        this->update();
    }
}
