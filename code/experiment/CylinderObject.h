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
        CylinderObject(CylinderObject &rObj);
        virtual ~CylinderObject(void);

        virtual TestObject * newObj(void);
        virtual TestObject * newObj(TestObject &rObject);
        virtual string getObjName(void);
        virtual UINT getObjID(void);
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(GLfloat delta);
        virtual void draw();

        virtual string genObjDesc(void); // get the object descriptions (fixed part)
        virtual string genObjPara(void); // get the object parameters (varied part)

        GLfloat radius;
        static const UINT objectID;

        // value range of the parameters
        vector<GLfloat> radiusRange;
    private:
        string objName;
};
