#include "pch.h"
#include "CGLRenderer.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <corecrt_math_defines.h>
#include <list>

struct Point3D
{
    float x;
    float y;
    float z;
};

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

    if (nPixelFormat == 0) return false;

    BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

    if (!bResult) return false;

    m_hrc = wglCreateContext(pDC->m_hDC);

    if (!m_hrc) return false;

    return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //-------------------------------
    glClearColor(1.0, 1.0, 1.0, 0.0);
    //-------------------------------
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    wglMakeCurrent(NULL, NULL);
    if (m_hrc)
    {
        wglDeleteContext(m_hrc);
        m_hrc = NULL;
    }
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //-------------------------------
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (double)w / (double)h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    //-------------------------------
    wglMakeCurrent(NULL, NULL);
}


//=============================================================================================
//=============================================================================================
//=============================================================================================

void DrawAxis(double w)
{
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f((float)w, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, (float)w, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, (float)w);
    glEnd();
}

void DrawGrid(double width, double height, int nSegW, int nSegH)
{
    double xInc = width / nSegW;
    double zInc = height / nSegH;

    double x = width / 2;
    double z = height / 2;

    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
        for (double i = -z; i <= 0 / 2; i +=zInc)
        {
            glVertex3d(x, 0.0, i);
            glVertex3d(-x, 0.0, i);
        }
        for (double i = 0; i <= z; i += zInc)
        {
            glVertex3d(x, 0.0, i);
            glVertex3d(-x, 0.0, i);
        }
        for (double i = -x; i <= 0; i += xInc)
        {
            glVertex3d(i, 0.0, z);
            glVertex3d(i, 0.0, -z);
        }
        for (double i = 0; i <= x; i += xInc)
        {
            glVertex3d(i, 0.0, z);
            glVertex3d(i, 0.0, -z);
        }
    glEnd();
}

void DrawCylinder(double h, double r1, double r2, int nSeg, float R, float G, float B)
{
    int j = 0;
    float angle = (2 * M_PI) / nSeg;
    float x, z;
    Point3D* base1;
    base1 = (Point3D*)calloc(nSeg, sizeof(Point3D));

    Point3D* base2;
    base2 = (Point3D*)calloc(nSeg, sizeof(Point3D));

    glShadeModel(GL_FLAT);
    glBegin(GL_POLYGON);
        glColor3f(R, G, B);
        for (float i = 0; i < 2 * M_PI; i+=angle)
        {
            x = r1 * cos(i);
            z = r1 * sin(i);
            base1[j] = { x, 0.0, z };
            j++;
            glVertex3f(x, 0.0, z);
        }
    glEnd();
    j = 0;
    glBegin(GL_POLYGON);
        glColor3f(R, G, B);
        for (float i = 0; i < 2 * M_PI; i += angle)
        {
            x = r2 * cos(i);
            z = r2 * sin(i);
            base2[j] = { x, (float)h, z };
            j++;
            glVertex3f(x, (float)h, z);
        }
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glColor3f(R, G, B);
        for (int i = 0; i < nSeg; i++)
        {
            x = base1[i].x;
            z = base1[i].z;
            glVertex3f(x, 0.0, z);
            x = base2[i].x;
            z = base2[i].z;
            glVertex3f(x, (float)h, z);
        }
    glEnd();

    //free(base1);
    //free(base2);
}

void DrawSphere(double r, int segAplha, int segBeta, float R, float G, float B)
{
    float anglAlpha = (2*M_PI) / segAplha;
    float anglBeta = (2*M_PI) / segBeta;

    float x, y, z;
    int j = 0;

    Point3D* upperPoints;
    upperPoints = (Point3D*)calloc(segAplha, sizeof(Point3D));
    Point3D* lowerPoints;
    lowerPoints = (Point3D*)calloc(segBeta, sizeof(Point3D));


    for (float i = 0; i < 2 * M_PI; i += anglBeta)
    {
        x = r * cos(0) * cos(i);
        y = r * sin(0);
        z = r * cos(0) * sin(i);
        lowerPoints[j] = { x,y,z };
        j++;
    }

    for (float a = anglAlpha; a < 2 * M_PI; a += anglAlpha)
    {
        y = r * sin(a);
        j = 0;
        for (float b = 0; b < 2 * M_PI+anglBeta; b += anglBeta)
        {
            x = r * cos(a) * cos(b);
            z = r * cos(a) * sin(b);
            upperPoints[j] = { x,y,z };
            j++;
        }
        glBegin(GL_QUAD_STRIP);
            glColor3f(R, G, B);
            for (int i = 0; i < segBeta; i++)
            {
                glVertex3f(lowerPoints[i].x, lowerPoints[i].y, lowerPoints[i].z);
                glVertex3f(upperPoints[i].x, upperPoints[i].y, upperPoints[i].z);
                lowerPoints[i] = upperPoints[i];
            }
        glEnd();
    }

    //free(&upperPoints);
    //free(&lowerPoints);
}

void DrawCone(double h, double r, int nSeg, float R, float G, float B)
{
    float angl = (2 * M_PI) / nSeg;
    Point3D* points;
    points = (Point3D*)calloc(nSeg, sizeof(Point3D));
    int j = 0;
    float x, z;

    for (float i = 0; i < 2 * M_PI; i += angl)
    {
        x = r * cos(i);
        z = r * sin(i);
        points[j] = { x, 0.0, z };
        j++;
    }
    
    glBegin(GL_POLYGON);
        glColor3f(R, G, B);
        for (int i = 0; i < nSeg; i++)
        {
            glVertex3f(points[i].x, 0.0, points[i].z);
        }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glColor3f(R, G, B);
        glVertex3f(0.0, (float)h, 0.0);
        for (int i = 0; i < nSeg; i++)
        {
            glVertex3f(points[i].x, 0.0, points[i].z);
        }
        glVertex3f(points[0].x, 0.0, points[0].z);
    glEnd();
    
    //free(&points);
}

void DrawFigure(double angle)
{
    DrawGrid(8.0, 8.0, 16, 16);
    DrawAxis(5.0);
    DrawCylinder(1.0, 0.4, 0.7, 10, 1.0, 0.0, 0.0);
    glTranslatef(0, 1.0, 0);
    DrawCylinder(1.0, 0.2, 0.2, 10, 0.0, 1.0, 0.0);
    glTranslatef(0, 1.2, 0);
    DrawSphere(0.2, 10, 10, 1.0, 1.0, 0.0);
    glPushMatrix();
        glRotatef(60 + angle, 1, 0, 0);
        glTranslatef(0, 0.2, 0);
        DrawCylinder(1.0, 0.2, 0.2, 10, 0.0, 1.0, 0.0);
    glPopMatrix();
    glPushMatrix();
        glRotatef(60 + angle, 1, 0, 0);
        glTranslatef(0, 1.4, 0);
        DrawSphere(0.2, 10, 10, 1.0, 1.0, 0.0);
    glPopMatrix();
    glTranslatef(0, 0.2, 0);
    DrawCylinder(1.0, 0.2, 0.2, 10, 0.0, 1.0, 0.0);
}

//==============================================================================================
//==============================================================================================
//==============================================================================================

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //-------------------------------
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
   ChangeLook(alpha, beta);
    
   DrawFigure(rotAngle);

    glFlush();
    //---------------------------------
    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::ChangeAngle(float angle, bool a)
{
    if (a)
    {
        alpha += angle;
    }
    else
    {
        beta += angle;
    }
}

void CGLRenderer::ChangeLook(float alpha, float beta)
{
    float x, y, z;
    double r = 15.0;
    x = r * cos(alpha) * cos(beta);
    y = r * sin(alpha);
    z = r * cos(alpha) * sin(beta);
    gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
}

void CGLRenderer::SetRot(float angl)
{
    rotAngle = angl;
}


