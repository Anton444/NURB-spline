#include "glwidget.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <windows.h>

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){}
float g_Points[6][3] = {
    {50, 100, 0},
    {150, 300, 0},
    {250, 350, 0},
    {400, 450, 0},
    {560, 400, 0},
    {660, 250, 0}
};

float g_Knots[] = {0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,3.0f,3.0f,3.0f,3.0f};
float g_Weight[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
int g_num_cvs=6;
int g_degree=3;
int g_order=g_degree+1;
int g_num_knots=g_num_cvs+g_order;
int LOD=6;
int hgt, wdh;

float CoxDeBoor(float u,int i,int k,const float* Knots) {
    if(k==1)
    {
        if( Knots[i] <= u && u <= Knots[i+1] ) {
            return 1.0f;
        }
        return 0.0f;
    }
    float Den1 = Knots[i+k-1] - Knots[i];
    float Den2 = Knots[i+k] - Knots[i+1];
    float Eq1=0,Eq2=0;
    if(Den1>0) {
        Eq1 = ((u-Knots[i]) / Den1) * CoxDeBoor(u,i,k-1,Knots);
    }
    if(Den2>0) {
        Eq2 = (Knots[i+k]-u) / Den2 * CoxDeBoor(u,i+1,k-1,Knots);
    }
    return Eq1+Eq2;
}

void GetOutpoint(float t,float OutPoint[]) {
    float valxNom = 0, valxDenom = 0, valyNom = 0, valyDenom = 0;
    for(unsigned int i=0;i!=g_num_cvs;++i) {
        valxNom += g_Weight[i]*CoxDeBoor(t, i, g_order, g_Knots)*g_Points[i][0];
        valxDenom += g_Weight[i]*CoxDeBoor(t, i, g_order, g_Knots);
        valyNom += g_Weight[i]*CoxDeBoor(t, i, g_order, g_Knots)*g_Points[i][1];
        valyDenom += g_Weight[i]*CoxDeBoor(t, i, g_order, g_Knots);
    }
    OutPoint[0] = valxNom/valxDenom;
    OutPoint[1] = valyNom/valyDenom;
    OutPoint[2] = 0;
}
void NURBS_spline () {
    glLoadIdentity();
    glColor3f(0,0,1);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i!=LOD;++i) {
        float t = g_Knots[g_num_knots-1] * i / (float)(LOD-1);

        if(i==LOD-1)
            t-=0.001f;
        float Outpoint[3]={0,0,0};
        GetOutpoint(t,Outpoint);
        glVertex3fv(Outpoint);
    }
    glEnd();
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i!=g_num_cvs;++i) {
        glVertex3fv(g_Points[i]);
    }
    glEnd();
    for(int i=0;i!=g_num_cvs;++i) {
        glPointSize(g_Weight[i]*10 + 1);
        glBegin(GL_POINTS);
        glColor3f(0.5 - i*0.07, 1 - i*0.17, i*0.15);
        glVertex3fv(g_Points[i]);
        glVertex3f(wdh - 20, hgt - 50 - 27*(i + 1), 0);
        glEnd();
    }
}

void GLWidget::initializeGL() {
    glClearColor(0.94, 0.94, 0.94, 0);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    NURBS_spline();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    hgt = h;
    wdh = w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0 , w, 0 , h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::mouseMoveEvent(QMouseEvent *me)
{
    this->x = me->x();
    this->y = abs(me->y() - this->height());
    if(this->changeNum == -1) {
        for(int i = 0; i < 7; i++) {
            if(this->x < g_Points[i][0] + 10 && this->x > g_Points[i][0] - 10 && this->y < g_Points[i][1] + 10 && this->y > g_Points[i][1] - 10) {
                this->changeNum = i;
            }
        }
    } else {
        glColor3f(0, 0, 0);
        glPointSize(8);
        glBegin(GL_POINT);
        glVertex3f(this->x, this->y, 0);
        glEnd();
        g_Points[changeNum][0] = this->x;
        g_Points[changeNum][1] = this->y;
    }
    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *me)
{
    this->changeNum = -1;
}

void GLWidget::changeWeight1(int w)
{
    g_Weight[0] = w*0.001;
    updateGL();
}
void GLWidget::changeWeight2(int w)
{
    g_Weight[1] = w*0.001;
    updateGL();
}
void GLWidget::changeWeight3(int w)
{
    g_Weight[2] = w*0.001;
    updateGL();
}
void GLWidget::changeWeight4(int w)
{
    g_Weight[3] = w*0.001;
    updateGL();
}
void GLWidget::changeWeight5(int w)
{
    g_Weight[4] = w*0.001;
    updateGL();
}
void GLWidget::changeWeight6(int w)
{
    g_Weight[5] = w*0.001;
    updateGL();
}

void GLWidget::lodChanged(int change)
{
    LOD = change*3;
    updateGL();
}
