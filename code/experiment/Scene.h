#pragma once

#include "stdafx.h"
#include "Screen.h"
#include <vector>
#include <hash_map>

using namespace stdext;

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
        static void dispatchTimerEvent(int timerID);
         
        static void reset();
        Screen& rScreen;

        inline const float getDuration() const {return this->_duration;}
        inline void setMinDuration(float minDuration) {this->_minDuration = minDuration;}

        inline const float getFps() const {return this->_fps; }

    protected:
        Scene();
        virtual BOOL renderScene() = 0;
        virtual BOOL reshape(int w, int h) = 0;
        virtual BOOL handleKeyboardEvent(unsigned char key, int x, int y) = 0;
        virtual BOOL handleKeyboardSpecialEvent(int key, int x, int y) = 0;
        virtual BOOL handleMouseEvent(int button, int state, int x, int y) = 0;
        virtual BOOL handleMouseMotionEvent(int x, int y) = 0;
        virtual BOOL handleMousePassiveMotionEvent(int x, int y) = 0;
        virtual BOOL handleTimerEvent(int timerID) = 0;
        virtual BOOL initDisplay() = 0;

        static int registerTimer();
        static void unregisterTimer(int timerID);
        static BOOL isRegisteredTimer(int timerID);
        static int getFreeTimerID();

        static Scene *currScene;
        static hash_map<int, BOOL> registeredTimerID;

        BOOL status;
        float _duration;
        float _minDuration;
        float _fps;

        LARGE_INTEGER _startTime;
        LARGE_INTEGER _endTime;
};
