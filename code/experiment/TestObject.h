#pragma once

#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;

class TestObject
{
    public:
        TestObject(void);
        virtual ~TestObject(void);
    
        virtual TestObject * newObj(void) = 0;
        virtual string getObjName(void) = 0;
        virtual UINT getObjID(void) = 0;
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(int delta) = 0;

        enum enumProj2DMode {ORTHOGONAL, PROJECTION};

        static const UINT objectID;

        //UINT texture; // FIX: how to represent texture here?
        
        enumProj2DMode proj2DMode;

        GLfloat slant;
        GLfloat height;
        GLfloat tilt;
        GLfloat initZAsptRatio;
        GLfloat adjZAsptRatio;
        GLfloat rotSpeed;
        GLfloat maxRotDeg;
      
        // Value range of the parameters
        vector<GLfloat> slantRange;
        vector<GLfloat> heightRange;
        vector<GLfloat> tiltRange;
        vector<GLfloat> initZAsptRatioRange;
        vector<GLfloat> rotSpeedRange;
        vector<GLfloat> maxRotDegRange;

    protected:
        string objName;
};
