#pragma once

#include "stdafx.h"
#include "texture.h"
#include <string>
#include <vector>

using namespace std;

class TestObject
{
    public:
        TestObject(vector<GLfloat>& slantRange,
                   vector<GLfloat>& heightRange,
                   vector<GLfloat>& tiltRange,
                   vector<GLfloat>& initZAsptRatioRange,
                   vector<GLfloat>& rotSpeedRange,
                   vector<GLfloat>& maxRotDegRange,
                   vector<texture_t *>& texs);

        TestObject(TestObject& rObj);
        virtual ~TestObject(void);
    
        virtual TestObject * newObj(TestObject& rObject) = 0;

        virtual string getObjName(void) = 0;
        virtual UINT getObjID(void) = 0;
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(GLfloat delta) = 0;
        virtual void draw(int drawStyle, BOOL enableTexture) = 0;

        virtual string genObjDesc(); // get the object descriptions (fixed part)
        virtual string genObjPara(); // get the object parameters (varied part)

        void reverseRotDirection();

        enum enumProj2DMode {ORTHOGONAL, PROJECTION};
        enum enumRotDirection {CLOCKWISE, COUNTERCLOCKWISE};

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

        GLfloat currRotDeg;
        enumRotDirection rotDirection;
      
        // Value range of the parameters
        vector<GLfloat> slantRange;
        vector<GLfloat> heightRange;
        vector<GLfloat> tiltRange;
        vector<GLfloat> initZAsptRatioRange;
        vector<GLfloat> rotSpeedRange;
        vector<GLfloat> maxRotDegRange;

        vector<texture_t *> textures;
    protected:
        string objName;
};
