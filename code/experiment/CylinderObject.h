#pragma once
#include "stdafx.h"
#include "testobject.h"
#include <string>

using namespace std;

class CylinderObject :
    public TestObject
{
    public:
        CylinderObject(void);
        virtual ~CylinderObject(void);

        virtual TestObject * newObj(void);
        virtual string getObjName(void);
        virtual UINT getObjID(void);
        virtual void setRandPara(void);

        GLfloat radius;

        //TODO:How to represent the range?
//        const GLfloat[] slant;
//        const GLfloat[] height;
//        const GLfloat[] tilt;
//        const GLfloat[] initZAsptRatio;
//        const GLfloat[] rotSpeed;
//        const GLfloat[] maxRotDeg;
    private:
        string objName;
};
