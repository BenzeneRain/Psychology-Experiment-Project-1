#pragma once
#include "mixedobjectviewscene.h"

class Overlapped2DViewScene :
    public MixedObjectViewScene
{
    public:
        Overlapped2DViewScene(void);
        Overlapped2DViewScene(TestObject &rObject);
        virtual ~Overlapped2DViewScene(void);

        virtual BOOL startScene();

    protected:
        TestObject *pObj;

    private:
        virtual BOOL renderScene(); 
        virtual BOOL reshape(int w, int h);
        virtual BOOL handleKeyboardEvent(unsigned char key, int x, int y);
        virtual BOOL handleKeyboardSpecialEvent(int key, int x, int y);
        virtual BOOL handleMouseEvent(int button, int state, int x, int y);
        virtual BOOL handleMouseMotionEvent(int x, int y);
        virtual BOOL handleMousePassiveMotionEvent(int x, int y);
        virtual BOOL handleTimerEvent(int timerID);
        virtual BOOL initDisplay(Screen& scr);
};
