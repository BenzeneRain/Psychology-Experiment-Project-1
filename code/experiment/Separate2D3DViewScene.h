#pragma once
#include "mixedobjectviewscene.h"
#include "Conditions.h"
#include "StereoViewingFrameV2.h"

class Separate2D3DViewScene :
    public MixedObjectViewScene
{
    public:
        Separate2D3DViewScene(cond_t& cond);
        virtual ~Separate2D3DViewScene(void);

        virtual BOOL startScene();

        const int ROTATION_TIMERID;
        const int SWITCH_TIMERID;
    private:
        virtual BOOL renderScene(); 
        virtual BOOL reshape(int w, int h);
        virtual BOOL handleKeyboardEvent(unsigned char key, int x, int y);
        virtual BOOL handleKeyboardSpecialEvent(int key, int x, int y);
        virtual BOOL handleMouseEvent(int button, int state, int x, int y);
        virtual BOOL handleMouseMotionEvent(int x, int y);
        virtual BOOL handleMousePassiveMotionEvent(int x, int y);
        virtual BOOL handleTimerEvent(int timerID);
        virtual BOOL initDisplay();

        inline BOOL _isStopConditionSatisfied() const 
        {
            LARGE_INTEGER lCurrent;
            QueryPerformanceCounter(&lCurrent);
            float fTime = (float)(lCurrent.QuadPart - this->_startTime.QuadPart) /
                (float)(this->rScreen.getCounterFrequency().QuadPart);

            if((this->rScreen.getFPS() != 0) && (fTime >= this->_minDuration))
                return TRUE;
            else
                return FALSE;
        }

        cond_t& condition;

        enum sepSceneStatus {DISPLAY_OBJECT, DISPLAY_BLACKSCREEN};
        typedef enum sepSceneStatus sepSceneStatus_t;
        sepSceneStatus_t currStatus;
};
