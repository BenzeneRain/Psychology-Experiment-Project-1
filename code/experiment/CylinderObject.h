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
        virtual BOOL adjustAsptRatio(int delta);

        GLfloat radius;
        static const UINT objectID;

        // value range of the parameters
        vector<GLfloat> radiusRange;
    private:
        string objName;
};
