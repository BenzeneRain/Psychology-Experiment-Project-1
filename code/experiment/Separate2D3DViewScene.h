#pragma once
#include "mixedobjectviewscene.h"

class Separate2D3DViewScene :
    public MixedObjectViewScene
{
    public:
        Separate2D3DViewScene(void);
        virtual ~Separate2D3DViewScene(void);

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
};
