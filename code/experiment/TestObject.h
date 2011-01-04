#pragma once

#include "stdafx.h"
#include <string>

using namespace std;

class TestObject
{
    public:
        TestObject(void);
        virtual ~TestObject(void);
    
        virtual TestObject * newObj(void) = 0;
        virtual string getObjName(void) = 0;
        virtual UINT getObjID(void) = 0;
        virtual void setRandPara(void) = 0;

        enum enumProj2DMode {ORTHOGONAL, PROJECTION};

        const UINT objectID;

        //UINT texture; // FIX: how to represent texture here?
        
        enumProj2DMode proj2DMode;

        GLfloat slant;
        GLfloat height;
        GLfloat tilt;
        GLfloat initZAsptRatio;
        GLfloat adjZAsptRatio;
        GLfloat rotSpeed;
        GLfloat maxRotDeg;

    protected:
        string objName;
};
