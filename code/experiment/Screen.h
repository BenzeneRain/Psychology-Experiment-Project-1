#pragma once

#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;

class Screen
{
    public:
        Screen(DEVMODE&);
        ~Screen();

        BOOL initGlut(UINT displayMode, string title, vector<HBITMAP>& hBitmaps);

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
        DEVMODE &rDevMode;

        int texNo;
        GLuint *texIDs;

        // the function is only for test purpose 
        static void testRenderScene();
    private:

        UINT displayMode;
        
};

