#pragma once
#include "purewordscene.h"
#include "Screen.h"

class PostExperimentScene :
    public PureWordScene
{
    public:
        PostExperimentScene(void);
        virtual ~PostExperimentScene(void);

        virtual BOOL startScene();

    private:
        virtual BOOL renderScene(); 
        virtual BOOL reshape(int w, int h);
        virtual string buildString();
        virtual BOOL handleKeyboardEvent(unsigned char key, int x, int y);
        virtual BOOL handleKeyboardSpecialEvent(int key, int x, int y);
        virtual BOOL handleMouseEvent(int button, int state, int x, int y);
        virtual BOOL handleMouseMotionEvent(int x, int y);
        virtual BOOL handleMousePassiveMotionEvent(int x, int y);
        virtual BOOL initDisplay(Screen& scr);
};
