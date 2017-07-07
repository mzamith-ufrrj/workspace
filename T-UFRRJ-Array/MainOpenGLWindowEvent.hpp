#ifndef _WINDOWS_HPP_
#define _WINDOWS_HPP_
#include <CellularAutomata.hpp>

struct stCAPosition{
    float x1, x2, y1, y2;
};

/*! \brief Main window class and its events
 * 
 */
class MainOpenGLWindowEvent{

private:
   static CellularAutomata *mCA;
   static stCAPosition mCAPosition;
   
   static float mWidth;
   static float mHeight;
   static bool  mAnimated;
   static void renderAutomatas(CellularAutomata *);
   
    
public:
   MainOpenGLWindowEvent(int ac, char **av, CellularAutomata *ca, float, float);
   ~MainOpenGLWindowEvent();
   static void render(void);
   static void viewPort(int, int);
   static void keyPress(unsigned char, int, int);
   static void specialKeyPress(int, int, int);
   static void mouseButton(int, int, int, int);
   static void mainloop(void);
   static void setPosition(stCAPosition );
   //static CellularAutomata * getCA(int index) {return mCA->getCA(index); }

   
};



#endif