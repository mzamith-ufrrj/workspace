#include <MainOpenGLWindowEvent.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;
CellularAutomata *MainOpenGLWindowEvent::mCA = NULL;
float MainOpenGLWindowEvent::mWidth = 0.0f;
float MainOpenGLWindowEvent::mHeight = 0.0f;
bool  MainOpenGLWindowEvent::mAnimated = false;
/*! Constructor of MainWindowEvents class 
 * 
 */ 
MainOpenGLWindowEvent::MainOpenGLWindowEvent(int ac, char **av, CellularAutomata *ca, float w, float h)
{
      mCA = ca;
      mWidth = w;
      mHeight = h;
      //getCA()->buildCells();
      cout << "All your base are belong to us!!!" << endl;
      glutInit(&ac, av);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
      glutInitWindowSize(800, 600);
      glutCreateWindow("Cellular Automata");
      glutDisplayFunc(this->render);
      glutReshapeFunc(this->viewPort);
      glutKeyboardFunc(this->keyPress);
      //glutSpecialFunc(this->specialKeyPress);
      //glutMouseFunc(this->mouseButton);
      //glutIdleFunc(this->mainloop);
      //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
      //Initializing opengl
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      
      glutMainLoop();
   
}

MainOpenGLWindowEvent::~MainOpenGLWindowEvent()
{}



void MainOpenGLWindowEvent::render(void){
   
   glClear(GL_COLOR_BUFFER_BIT);
    
   renderAutomatas(mCA);
    
   //renderAutomatas();
   glutSwapBuffers();
   
}

void MainOpenGLWindowEvent::viewPort(int w, int h){
   if(h == 0) h = 1;

   /*
   mPointSize = static_cast<float>(w) / getCA()->getGrid()->getCellXf();
   
   if (mPointSize > (static_cast<float>(h) / getCA()->getGrid()->getCellYf()))
      mPointSize = static_cast<float>(h) / getCA()->getGrid()->getCellYf();
   
   //mPointSize = mPointSize * (1.0f - 0.1f);
   */

   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0.0f, mWidth, 0.0f, mHeight);
   //glOrtho (-0.01f, getCA()->getGrid()->getCellXf() + 0.01f, -0.01f, getCA()->getGrid()->getCellYf() + 0.01f, 0.0f, 1.0f);
   
   glutPostRedisplay();
}


void MainOpenGLWindowEvent::keyPress(unsigned char key, int x, int y){
   switch (key){
      case ' ':mCA->update();break;
      case 27: exit(EXIT_SUCCESS);break; 
      //case '1':mCA->debugCells();break;
      case 't':
      case 'T': mAnimated = !mAnimated;break;

   }
   glutPostRedisplay();
}

void MainOpenGLWindowEvent::specialKeyPress(int key, int x, int y){
    switch (key){
        case 5:
            
            
            break;
            
    }
    glutPostRedisplay();
}

void MainOpenGLWindowEvent::mouseButton(int button, int state, int x, int y){
   
}

void MainOpenGLWindowEvent::mainloop(void){
    if (mAnimated) mCA->update();
    char txt[256];
    //printf("%s", x?"true":"false");
   //sprintf(txt, "Animated: %s | Timestep %u", mAnimated?"TRUE":"FALSE", mCA->getTimestep());
    glutSetWindowTitle(txt);
    glutPostRedisplay();
}

void MainOpenGLWindowEvent::renderAutomatas(CellularAutomata *ca){
  /*
    glPushMatrix();
   
   float normax =ca->getGrid()->getCellXf();
   float normay = ca->getGrid()->getCellYf();
   float x1 = mCAPosition.x1;
   float y1 = mCAPosition.y1;
   float x2 = mCAPosition.x2;
   float y2 = mCAPosition.y2;
   float deltaX = fabs(x2 - x1) / normax;
   float deltaY = fabs(y2 - y1) / normay;
   
   
   for (int j = 0 ; j < ca->getGrid()->getCellYi(); j++){
      for (int i = 0 ; i < ca->getGrid()->getCellXi(); i++){
        bool c =  ca->getGrid()->getGrid_boolean(i, j);
        float u = static_cast<float> (i) * deltaX;
        float v = static_cast<float> (j) * deltaY;

        if (!c){
            glColor3f(0.0f, 1.0f, 0.0f);
        }else{
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        glBegin(GL_QUADS); //GL_QUADS);GL_LINE_LOOP

        glVertex3f(x1 + u, y1 + v, 0.0f);
        glVertex3f(x1 + u + deltaX, y1 + v, 0.0f);
        glVertex3f(x1 + u + deltaX, y1 + v + deltaY, 0.0f);
        glVertex3f(x1 + u, y1 + v + deltaY, 0.0f);
        
        
        
        //glTranslatef(0.5f, -0.5f, 0.0f);

        glEnd();
    
      }
   }

   
   glPopMatrix();
   */
}

//void MainOpenGLWindowEvent::setPosition(stCAPosition caposition){ mCAPosition = caposition; }