#include "canvasopengl.h"
#include <cmath>
#define PI 3.14159265



//-------------------------------------------------------------------------------------------

Angles angles; //global variable
Vector v1, v2;
Point originPoint;

//-------------------------------------------------------------------------------------------
 Vector getVector(Point p1, Point p2){
    Vector v1;
    v1.x = p2.x - p1.x;
    v1.y = p2.y - p1.y;
    v1.z = p2.z - p1.z;
    return v1;

}
//-------------------------------------------------------------------------------------------
Plane planeEquation(Vector a, Vector b, Point planPoint){
    //making cross product
    // | i  j  k  |
    // | xa ya za |
    // | xb yb zb |

    Plane p;
    p.Xcoef = ( (a.y * b.z) - (a.z * b.y) ); // X coeficient (i)
    p.Ycoef = ( (a.z * b.x) - (a.x * b.z) ); // Y coeficient (j)
    p.Zcoef = ( (a.x * b.y) - (a.y * b.x) ); // Z coeficient (k)

    p.independentTerm = (-1)*(p.Xcoef*planPoint.x + p.Ycoef*planPoint.y + p.Zcoef*planPoint.z);

    return p;
}
//-------------------------------------------------------------------------------------------
float getAngle(Vector normal, Vector axis){
    float cosine, angle, dotProduct, moduleNormal, moduleAxis;

    dotProduct = normal.x*axis.x + normal.y*axis.y + normal.z*axis.z;
    moduleNormal = sqrt(pow(normal.x, 2)+pow(normal.y, 2)+pow(normal.z, 2));
    moduleAxis = sqrt(pow(axis.x, 2)+pow(axis.y, 2)+pow(axis.z, 2));

    cosine = dotProduct/(moduleNormal*moduleAxis);
    angle = acos(cosine);
    return angle; //in radians

}
//-------------------------------------------------------------------------------------------
//PARAMS: angle:        angle to rotate
//         vec:         coordinates of rotated vector in space
//         direction:   0: sentido horario, 1: sentido anti horario

Vector rotateInX(float angle, Vector vec, char direction){
    Vector newVec;

    //open rotation matrix;
    newVec.x = vec.x;
    newVec.y = direction == 0? vec.y*cos(angle)-vec.z*sin(angle) : vec.y*cos(angle)+vec.z*sin(angle);
    newVec.z = direction == 0? vec.y*sin(angle) + vec.z*cos(angle) : (-1)*vec.y*sin(angle) + vec.z*cos(angle);

    return newVec;
}
//-------------------------------------------------------------------------------------------
//rotacao no sentido anti-horario
Vector rotateInY(float angle, Vector vec, char direction){
    Vector newVec;

    newVec.x = direction == 0? vec.x*cos(angle) - vec.z*sin(angle) : vec.x*cos(angle) + vec.z*sin(angle);
    newVec.y = vec.y;
    newVec.z = direction == 0? vec.x*sin(angle) + vec.z*cos(angle) : (-1)*vec.x*sin(angle) + vec.z*cos(angle);

    return newVec;
}
//-------------------------------------------------------------------------------------------
 void findRotatioAngles(Point p1, Point p2, Point p3){

    //getting vectors (already with origins in (0,0,0) ):
    v1 = getVector(p1, p2);
    v2 = getVector(p1, p3);

    //plan which contains all 3 point. (last argument could be p1, p2 or p3 - Any point in plane)
    Plane plane = planeEquation(v1, v2, p1);

    //get normal od both plenes:
    Vector normal;
    normal.x = plane.Xcoef; normal.y = plane.Ycoef; normal.z = plane.Ycoef;

    //***********************************************************
    //to get the angle between 'normal' and 'x-axis', rotate in Y
    //to get the angle between 'normal' and 'y-axis', rotate in X
    Vector x_axis, y_axis;
    x_axis.x = 1; x_axis.y = 0; x_axis.z = 0;
    y_axis.x = 0; y_axis.y = 1; y_axis.z = 0;

    //calculate angle between normal and x_axis and y_axis;
    angles.x_axis = getAngle(normal, x_axis);
    angles.y_axis = getAngle(normal, y_axis);


}
//-------------------------------------------------------------------------------------------
Point calculate(char direction, Vector oldVec){
    Vector newVec;
    Point p;

    newVec = rotateInX((PI/2) - angles.x_axis, oldVec, direction);
    newVec = rotateInY((PI/2) - angles.y_axis, newVec, direction);

    p.x = newVec.x; p.y = newVec.y; p.z = newVec.z;
}
//-------------------------------------------------------------------------------------------
Point* change(Point p1, Point p2, Point p3){

    findRotatioAngles(p1,p2,p3);
    originPoint = p1;

    Point new_p1, new_p2, new_p3;
    new_p1.x = 0; new_p1.y = 0; new_p1.z = 0;
    new_p2 = calculate(0, v1);
    new_p3 = calculate(0, v2);

    Point points[3];
    points[0] = new_p1; points[1] = new_p2; points[2] = new_p3;

    return points;
}
//-------------------------------------------------------------------------------------------
Point changeBack(Vector vec){
    Point newPoint = calculate(1, vec);
    newPoint.x += originPoint.x;
    newPoint.y += originPoint.y;
    newPoint.z += originPoint.z;
    return newPoint;
}


void CanvasOpenGL::doScanLine(std::map<int,std::list<Edge>> ET){
    std::list<Edge> AET;
    int scanLine = ET.begin()->first;
    int yMax = 0;
    for (std::map<int, list<Edge>>::iterator it = ET.begin(); it != ET.end(); it++)
    {
        for (list<Edge>::iterator iterator = it->second.begin(), end = it->second.end(); iterator != end; ++iterator)
        {
            if (iterator->yMax > yMax)
            {
                yMax = iterator->yMax;
            }
        }
    }

    for (; scanLine < yMax; scanLine++)
    { // scanlines
        if (ET.find(scanLine) != ET.end())
        {
            AET.splice(AET.end(), ET.find(scanLine)->second);
        }
        for (std::list<Edge>::iterator iterator = AET.begin(), end = AET.end(); iterator != end; ++iterator)
        {
            if (iterator->yMax == scanLine)
            {
                AET.remove(*iterator);
            }
            else
            {
                iterator->x += iterator->xInc;
            }
        }
        AET.sort([](Edge lhs, Edge rhs) { return lhs.x < rhs.x; });
        float first = 0;
        for (std::list<Edge>::const_iterator iterator = AET.begin(), end = AET.end(); iterator != end; ++iterator)
        {
            if (int(first) == 0)
            {
                first = iterator->x;
            }
            else
            {
                for (int i = int(ceil(first)); i < floor(iterator->x); i++)
                { // fill the scanline
                    //painter.drawPoint(i, scanLine);
                    Vector v;
                    v.x = i;
                    v.y = scanLine;
                    v.z = 0;
                    //OPENGL
                    this->pointsToPaint.push_back(changeBack(v));

                }
                first = 0;
            }
        }
    }
}

void CanvasOpenGL::scanLine(Point points[3]){
    QPoint p1(points[0].x,points[0].y),p2(points[1].x,points[1].y),p3(points[2].x,points[2].y);
    Edge e1(p1,p2),e2(p2,p3),e3(p1,p3);
    std::map<int,std::list<Edge>> ET;
    this->doScanLine(ET);
}
//-------------------------------------------------------------------------------------------



CanvasOpenGL::CanvasOpenGL(QWidget *parent) : QOpenGLWidget(parent)
{
    this->draw = false;
    this->radius = 12;
    this->r = 255;
    this->g = 255;
    this->b = 255;

    readInputFile();
    printHeightMap();
}

void CanvasOpenGL::printHeightMap(){
    Point p1,p2,p3;
    Point *points;
    for(int i=0;i<254;i++){
        for(int j=0;j<254;j++){
            p1.x = p2.x = p3.x = i;
            p1.y = p3.y = p2.y = j;
            p3.y++;
            p2.x++;
            p1.z = this->heightmap[i][j];
            p2.z = this->heightmap[i][j+1];
            p3.z = this->heightmap[i+1][j];
            points = change(p1,p2,p3);
            scanLine(points);
        }
    }
    this->update();
}


void CanvasOpenGL::readInputFile(){

    string line;
    ifstream myfile("C:\\Users\\LuiRocha\\Desktop\\t2-cg\\heightmap.txt");
    if (myfile.is_open())
    {
        int j=0;
        while (getline(myfile, line))
        {
            std::string::size_type sz = 0;     // alias of size_t
            for(int i=0;i<255;i++){
                this->heightmap[j][i] = std::stof (line,&sz);
                line = line.substr(sz);
                std::cerr << this->heightmap[j][i];
            }
            j++;
        }
        myfile.close();
    }
}

CanvasOpenGL::~CanvasOpenGL()
{
    qDebug("esd");
    this->draw = false;
    this->radius = 12;
    this->width = 800;
    this->height = 600;
    this->r = 255;
    this->g = 255;
    this->b = 255;
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
    (void)vertices;
}

void CanvasOpenGL::fillPoligon(PolygonS *polygon)
{
    (void)polygon;
}

GLdouble CanvasOpenGL::euclidean(QVector3D p1, QVector3D p2)
{
    GLdouble result = GL_DOUBLE;
    (void)p1;
    (void)p2;
    return result;
}

void CanvasOpenGL::reset()
{
}

void CanvasOpenGL::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_PROGRAM_POINT_SIZE);

    //this->isPerspective = false;

    this->aspect = this->width/this->height;

    this->fovY = 100.0;

    this->resetParameters();

    glViewport(0, 0, this->width, this->height);

}
void CanvasOpenGL::resizeGL(GLint w, GLint h)
{
    (void)w;
    (void)h;
}

void CanvasOpenGL::paintGL()
{
    cerr << "a";

    this->setParameters();

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glPointSize(1.0);
    glBegin(GL_POINTS);

    for (vector<Point>::iterator i = this->pointsToPaint.begin(); i != this->pointsToPaint.end(); i++) {

    }
    glEnd();

}

void CanvasOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
}

void CanvasOpenGL::mousePressEvent(QMouseEvent *event)
{
    this->update();
}

void CanvasOpenGL::wheelEvent(QWheelEvent *event)
{
    (void)event;
}

void CanvasOpenGL::clearScreen()
{
    this->ET.clear();
    this->AET.clear();

    this->draw = false;

    this->first.setX(0);
    this->first.setY(0);

    this->last.setX(0);
    this->last.setY(0);

    if (!this->vertices.empty())
    {
        this->vertices.clear();
        this->update();
    }
}
