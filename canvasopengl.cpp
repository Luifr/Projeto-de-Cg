#include "canvasopengl.h"
#include <cmath>


double angle; //global variable
Vector *v1, *v2, *versor, *normal;
Point originPoint;
double a, b, c, d, e, f, g, h, i;
char inv_normal = 0;

//-------------------------------------------------------------------------------------------
 Vector* getVector(Point p1, Point p2){
    Vector* v1 = (Vector*)malloc(sizeof(Vector));
    v1->x = p2.x - p1.x;
    v1->y = p2.y - p1.y;
    v1->z = p2.z - p1.z;
    return v1;
}
//-------------------------------------------------------------------------------------------
Vector* planeEquation(Vector *a, Vector *b){
    //making cross product
    // | i  j  k  |
    // | xa ya za |
    // | xb yb zb |

    Vector* p = (Vector*)malloc(sizeof(Vector));
    p->x = ( (a->y * b->z) - (a->z * b->y) ); // X coeficient (i)
    p->y = ( (a->z * b->x) - (a->x * b->z) ); // Y coeficient (j)
    p->z = ( (a->x * b->y) - (a->y * b->x) ); // Z coeficient (k)

    return p;
}
//-------------------------------------------------------------------------------------------
double getAngle(Vector normal, Vector axis){ //zaxis
    double cosine, angle, dotProduct, moduleNormal, moduleAxis;

    dotProduct = normal.x*axis.x + normal.y*axis.y + normal.z*axis.z;
    moduleNormal = sqrt(pow(normal.x, 2)+pow(normal.y, 2)+pow(normal.z, 2));
    moduleAxis = sqrt(pow(axis.x, 2)+pow(axis.y, 2)+pow(axis.z, 2));

    cosine = dotProduct/(moduleNormal*moduleAxis);
    angle = acos(cosine);

    return angle; //in radians
}
//-------------------------------------------------------------------------------------------
void setMatrix(double angle, Vector versor){
    double co = cos(angle);
    double s = sin(angle);

    if(versor.x == 0 && versor.y == 0 && versor.z == 0){
        a=b=c=d=e=f=g=h=i=0;
        return;
    }

    double normalize = sqrt(pow(versor.x, 2)+ pow(versor.y , 2)+ pow(versor.z, 2));
    double x = versor.x/normalize, y = versor.y/normalize, z = versor.z/ normalize;

    a = pow(x, 2)*(1-co) + co;    b = x*y*(1-co)-(z*s);        c = x*z*(1-co)+(y*s);
    d = y*x*(1-co) + (z*s);       e = pow(y, 2)*(1-co) + co;   f = y*z*(1-co) - (x*s);
    g = x*z*(1-co) - (y*s);       h = y*z*(1-co) + (x*s);      i = pow(z, 2)*(1-co) + co;
}
//-------------------------------------------------------------------------------------------
Point* rotateToZ(Point point){
    Point* newPoint = (Point*) malloc(sizeof(Point));
    //open rotation matrix;
    cerr << g << " " << e << endl;
    newPoint->x = a*point.x + b*point.y + c*point.z;
    newPoint->y = d*point.x + e*point.y + f*point.z;
    newPoint->z = g*point.x + h*point.y + i*point.z;
    cerr << newPoint->y << endl;

    return newPoint;
}
//-------------------------------------------------------------------------------------------
Vector* invertNormal(){
        normal->x = -normal->x;
        normal->y = -normal->y;
        normal->z = -normal->z;
        inv_normal = inv_normal==0? 1 : 0;
}
//-------------------------------------------------------------------------------------------
 void findRotatioAngles(Point p1, Point p2, Point p3){
    //getting vectors (already with origins in (0,0,0) ):
    v1 = getVector(p1, p2);
    v2 = getVector(p1, p3);

    //plan which contains all 3 point. (last argument could be p1, p2 or p3 - Any point in plane)
    //already gets normal:
    normal = planeEquation(v1, v2);
    if(normal->z < 0) invertNormal();

    //eixo z:
    Vector z_axis;
    z_axis.x = 0; z_axis.y = 0; z_axis.z = 1;

    //angulo entre a normal e o eixo z:
    angle = getAngle(*normal, z_axis);
    //achando o versor entre a normal e o eixo z:
    versor = planeEquation(normal, &z_axis);


}
//-------------------------------------------------------------------------------------------
Point** change(Point p1, Point p2, Point p3){
    findRotatioAngles(p1,p2,p3);
    originPoint = p1;
    Point** newPoints = (Point**)malloc(3*sizeof(Point*));

    //setando a matriz de rotação:

    setMatrix(angle, *versor);

    //rotacionando os 3 pontos em Z:
    Point p1_new, p2_new, p3_new;
    p1_new.x = 0; p1_new.y = 0; p1_new.z = 0;
    p2_new.x = v1->x; p2_new.y = v1->y; p2_new.z = v1->z;
    p3_new.x = v2->x; p3_new.y = v2->y; p3_new.z = v2->z;

    //cerr << p2_new.x << " " << p2_new.y << endl;

    newPoints[0] = rotateToZ(p1_new);
    newPoints[1] = rotateToZ(p2_new);
    newPoints[2] = rotateToZ(p3_new);

    return newPoints;
}
//-------------------------------------------------------------------------------------------
Point* changeBack(Point p){
    Point* newPoint = (Point*)malloc(sizeof(Point));

    //setando a matriz de rotação (desrotacionar oq foi rotacionado):
    Vector* invertedVersor = (Vector*)malloc(sizeof(Vector));
    invertedVersor->x = versor->x;//  |
    invertedVersor->y = versor->y;//  |--> invertando (normal X z-axis)
    invertedVersor->z = versor->z;//  |

    setMatrix(-angle, *invertedVersor);
    //rotacionando o ponto em Z:
    newPoint = rotateToZ(p);

    newPoint->x += originPoint.x;
    newPoint->y += originPoint.y;
    newPoint->z += originPoint.z;

    free(v1);
    free(v2);

    return newPoint;
}

//////////////////////////////////////////////////////////////////////////////

void CanvasOpenGL::doScanLine(std::map<int,std::list<Edge>> ET){
    std::list<Edge> AET;
    int scanLine = ET.begin()->first;
    //cerr << scanLine << endl;
    return;
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
        //cerr << scanLine << " " << yMax << endl;
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
                    Point v;
                    v.x = i;
                    v.y = scanLine;
                    v.z = 0;
                    //OPENGL
                    this->pointsToPaint.push_back(*changeBack(v));

                }
                first = 0;
            }
        }
    }
}

void CanvasOpenGL::scanLine(Point** points){
    QPoint p1(points[0]->x,points[0]->y),p2(points[1]->x,points[1]->y),p3(points[2]->x,points[2]->y);
    int yMin1 = min(p1.y(),p2.y()),yMin2 = min(p3.y(),p2.y()),yMin3 = min(p1.y(),p3.y());
    Edge e1(p1,p2),e2(p2,p3),e3(p1,p3);
    std::map<int,std::list<Edge>> ET;
    list<Edge> l1,l2,l3;
    l1.push_front(e1);
    //cerr << points[1]->x << " " << points[1]->y << " " << points[2]->x << " " << endl;
    return;
    if(yMin1 == yMin2){ // 1 = 2 = 3, 1 = 2, 1 = 3, 2 = 3, != ,
        l1.push_front(e2);
        if(yMin3 == yMin1){
            l1.push_front(e3);
        }
        else{
            l3.push_front(e3);
            ET[yMin3] = l3;
        }
    }
    else if(yMin2 == yMin3){
        l2.push_front(e2);
        l2.push_front(e3);
        ET[yMin2] = l2;
    }
    else if(yMin1 == yMin3){
        l1.push_front(e3);
        l2.push_front(e2);
        ET[yMin2] = l2;
    }
    else{
        l2.push_front(e2);
        l3.push_front(e3);
        ET[yMin2] = l2;
        ET[yMin3] = l3;
    }


    ET[yMin1] = l1;

    this->doScanLine(ET);
}
//-------------------------------------------------------------------------------------------



CanvasOpenGL::CanvasOpenGL(QWidget *parent) : QOpenGLWidget(parent)
{
    this->width = 800;
    this->height = 600;
    this->draw = false;
    this->radius = 12;
    this->r = 255;
    this->g = 255;
    this->b = 255;
    readInputFile();
    printHeightMap();
}

void CanvasOpenGL::printHeightMap(){
    Point p1,p2,p3,p4;
    Point **points;
    for(int i=0;i<254;i++){
        for(int j=0;j<254;j++){
            p1.x = p2.x = p3.x = p4.x = i;
            p1.y = p3.y = p2.y = p4.y = j;
            p3.y++;
            p2.x++;
            p4.y++;
            p4.x++;
            p1.z = this->heightmap[i][j];
            p2.z = this->heightmap[i][j+1];
            p3.z = this->heightmap[i+1][j];
            p4.z = this->heightmap[i+1][j+1];
            points = change(p1,p2,p3);
            scanLine(points);
            free(points[0]);
            free(points[1]);
            free(points[2]);
            free(points);
            points = change(p2,p3,p4);
            scanLine(points);
            free(points[0]);
            free(points[1]);
            free(points[2]);
            free(points);
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
            }
            j++;
        }
        myfile.close();
    }
}

CanvasOpenGL::~CanvasOpenGL()
{

}

void CanvasOpenGL::setParameters()
{
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

void CanvasOpenGL::resetParameters()
{
    this->hMin = -400;
    this->hMax = 400;
    this->vMin = -300;
    this->vMax = 300;
    this->mNear = -500.0;
    this->mFar = 500.0;

    this->xRot = 0.0;
    this->yRot = 0.0;
    this->zRot = 0.0;
}

void CanvasOpenGL::toggleProjection()
{
}

void CanvasOpenGL::perspectiveGL()
{
    GLdouble fW, fH;

    fH = tan(this->fovY / 360.0 * this->pi) * this->nearV;
    fW = fH * this->aspect;

    glFrustum(-fW, fW, -fH, fH, this->nearV, this->farV);
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

    this->isPerspective = false;

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
    this->setParameters();

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glPointSize(10.0);
    glBegin(GL_POINTS);

    glVertex3i(0, 0, 0);

    for (vector<Point>::iterator i = this->pointsToPaint.begin(); i != this->pointsToPaint.end(); i++) {
        glColor3f(i->z, 0.0f, 1.0-i->z);
        glVertex3f(GLfloat(i->x), GLfloat(i->y), GLfloat(i->z));
    }

    glEnd();

    glFlush();

}

void CanvasOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
}

void CanvasOpenGL::mousePressEvent(QMouseEvent *event)
{
    //this->update();
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
