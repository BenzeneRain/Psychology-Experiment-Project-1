#pragma once
#include "stdafx.h"
#include "testobject.h"
#include "texture.h"
#include <string>

using namespace std;

class CylinderObject :
    public TestObject
{
    public:
        CylinderObject(vector<GLfloat>& slantRange,
                vector<GLfloat>& heightRange,
                vector<GLfloat>& tiltRange,
                vector<GLfloat>& initZAsptRatioRange,
                vector<GLfloat>& rotSpeedRange,
                vector<GLfloat>& maxRotDegRange,
                vector<texture_t *>& texs,
                vector<GLfloat>& radiusRange);
        CylinderObject(CylinderObject &rObj);
        virtual ~CylinderObject(void);

        virtual TestObject * newObj(TestObject &rObject);

        virtual string getObjName(void);
        virtual UINT getObjID(void);
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(GLfloat delta);
        virtual void draw(int drawStyle, BOOL enableTexture);

        virtual string genObjDesc(void); // get the object descriptions (fixed part)
        virtual string genObjPara(void); // get the object parameters (varied part)

        GLfloat radius;
        static const UINT objectID;

        GLuint topTextureID;
        GLuint bottomTextureID;
        GLuint sideTextureID;

        // value range of the parameters
        vector<GLfloat> radiusRange;
    private:
        string objName;
};
