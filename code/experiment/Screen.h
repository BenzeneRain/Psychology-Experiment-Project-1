#ifndef _H_SCREEN_
#define _H_SCREEN_
#include "stdafx.h"
#include <string>
using namespace std;

class Screen
{
    public:
        Screen();
        ~Screen();

        BOOL initGlut(DEVMODE devMode,
            UINT displayMode, string title);

        BOOL cancelKMBinds();
        BOOL clear();
        BOOL displayString(string str, float x, float y);
        BOOL setDisplayFunc(void (*displayFunc)(void));
        BOOL setReshapeFunc(void (*func)(int w, int h));
        BOOL setKeyboardFunc(void (*func)(unsigned char key, int x, int y));
        BOOL setKeyboardSpecialFunc(void (*func)(int key, int x, int y));
        BOOL setMouseFunc(void (*func)(int button, int state, int x, int y));
        BOOL setMouseMotionFunc(void (*func)(int x, int y));
        BOOL setMousePassiveMotionFunc(void (*func)(int x, int y));

        BOOL run();

        void render();

        BOOL stopped;

        // the function is only for test purpose 
        static void testRenderScene();
    private:

        UINT displayMode;
        
};

#endif /* _H_SCREEN_ */
