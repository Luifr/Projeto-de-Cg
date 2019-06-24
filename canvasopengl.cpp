#include "canvasopengl.h"

CanvasOpenGL::CanvasOpenGL(QWidget *parent) : QOpenGLWidget(parent) {
    this->draw = false;
    this->radius = 12;
    this->r = 255;
    this->g = 255;
    this->b = 255;

    this->width = 800;
    this->height = 600;
}

CanvasOpenGL::~CanvasOpenGL(){}

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

void CanvasOpenGL::setG(int val){
    this->g = val;
    if(!draw)
        this->update();
}

void CanvasOpenGL::setB(int val){
    this->b = val;
    if(!draw)
        this->update();
}

void CanvasOpenGL::setR(int val){
    this->r = val;
    if(!draw)
        this->update();
}




void CanvasOpenGL::perspectiveGL() {
    GLdouble fW, fH;

    fH = tan(this->fovY / 360.0 * this->pi) * this->nearV;
    fW = fH * this->aspect;

    glFrustum(-fW, fW, -fH, fH, this->nearV, this->farV);
}

void CanvasOpenGL::toggleProjection() {
    this->isPerspective = !this->isPerspective;

    this->update();
}

void CanvasOpenGL::initializeGL() {
    glEnable(GL_DEPTH_TEST);

    this->isPerspective = false;

    this->aspect = this->width/this->height;

    this->fovY = 100.0;

    this->resetParameters();

    glViewport(0, 0, this->width, this->height);
}

//void CanvasOpenGL::paintGL() {

//    this->setParameters();

//    glMatrixMode(GL_MODELVIEW);

//    glLoadIdentity();

//    glClearColor(1.0, 1.0, 1.0, 0.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//    glRotated(this->xRot, 1.0, 0.0, 0.0); // X
//    glRotated(this->yRot, 0.0, 1.0, 0.0); // Y
//    glRotated(this->zRot, 0.0, 0.0, 1.0); // Z

//    glBegin(GL_QUADS);
//    for (int i = 0; i < 6; i++) {
//        for(int j = 0; j < 4; j++) {
//            glColor3fv(this->colors[i]);
//            glVertex3fv(this->faces[i][j]);
//        }
//    }
//    glEnd();

//    glFlush();
//}

void CanvasOpenGL::resizeGL(GLint w, GLint h) {
    this->width = w;
    this->height = h;
    this->aspect = this->width/this->height;

    this->update();
}

void CanvasOpenGL::setParameters () {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (this->isPerspective) {
        this->perspectiveGL();
    } else {
        glOrtho(
            this->hMin,
            this->hMax,
            this->vMin,
            this->vMax,
            this->nearV,
            this->farV
        );
    }
}

void CanvasOpenGL::resetParameters () {
    this->hMin = -50.0;
    this->hMax = 50.0;
    this->vMin = -50.0;
    this->vMax = 50.0;
    this->nearV = 10.0;
    this->farV = 50.0;

    this->xRot = 0.0;
    this->yRot = 0.0;
    this->zRot = 0.0;
}

void CanvasOpenGL::reset() {
    this->resetParameters();

    this->update();
}

void CanvasOpenGL::setvMax (GLdouble arg1) {
    this->vMax = arg1;

    this->update();
}

void CanvasOpenGL::setvMin (GLdouble arg1) {
    this->vMin = arg1;

    this->update();
}

void CanvasOpenGL::sethMax (GLdouble arg1) {
    this->hMax = arg1;

    this->update();
}

void CanvasOpenGL::sethMin (GLdouble arg1) {
    this->hMin = arg1;

    this->update();
}

void CanvasOpenGL::setFar (GLdouble arg1) {
    this->farV = arg1;

    this->update();
}

void CanvasOpenGL::setNear (GLdouble arg1) {
    this->nearV = arg1;

    this->update();
}

void CanvasOpenGL::setFovY (GLdouble arg1) {
    this->fovY = arg1;

    this->update();
}

//void CanvasOpenGL::mouseMoveEvent(QMouseEvent *event) {
//    GLint dx = event->x() - this->lastPos.x();
//    GLint dy = event->y() - this->lastPos.y();

//    if (event->buttons() & Qt::LeftButton) {
//        this->xRot = this->xRot + 2 * dy;
//        this->yRot = this->yRot + 2 * dx;
//    } else if (event->buttons() & Qt::RightButton) {
//        this->xRot = this->xRot + 2 * dy;
//        this->zRot = this->zRot + 2 * dx;
//    }
//    this->lastPos = event->pos();

//    this->update();
//}



