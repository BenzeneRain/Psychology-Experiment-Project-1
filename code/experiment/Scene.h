#pragma once

#include "Screen.h"
#include <vector>

using namespace std;

class Scene
{
    public:
        virtual ~Scene(void) = 0;
        virtual BOOL startScene() = 0;

        static void dispatchSceneRender();
        static void dispatchReshape(int w, int h);
        static void dispatchKeyboardEvent(unsigned char key, int x, int y);
        static void dispatchKeyboardSpecialEvent(int key, int x, int y);
        static void dispatchMouseEvent(int button, int state, int x, int y);
        static void dispatchMouseMotionEvent(int x, int y);
        static void dispatchMousePassiveMotionEvent(int x, int y);
         
        vector<Screen *> screens;

    protected:
        Scene();
        virtual BOOL renderScene() = 0;
        virtual BOOL reshape(int w, int h) = 0;
        virtual BOOL handleKeyboardEvent(unsigned char key, int x, int y) = 0;
        virtual BOOL handleKeyboardSpecialEvent(int key, int x, int y) = 0;
        virtual BOOL handleMouseEvent(int button, int state, int x, int y) = 0;
        virtual BOOL handleMouseMotionEvent(int x, int y) = 0;
        virtual BOOL handleMousePassiveMotionEvent(int x, int y) = 0;

        static Scene *currScene;
};
