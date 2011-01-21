#pragma once

#include "stdafx.h"
#include "texture.h"
#include "rangeType.h"
#include <string>
#include <vector>

using namespace std;

class TestObject
{
    public:
        TestObject(rangeType<GLfloat>& pitchRange,
                   rangeType<GLfloat>& yawRange,
                   rangeType<GLfloat>& rollRange,
                   rangeType<GLfloat>& heightRange,           
                   rangeType<GLfloat>& initZAsptRatioRange,
                   rangeType<GLfloat>& rotSpeedRange,
                   rangeType<GLfloat>& maxRotDegRange,
                   vector<texture_t *>& texs);

        TestObject(TestObject& rObj);
        virtual ~TestObject(void);
    
        virtual TestObject * newObj(TestObject& rObject) = 0;

        virtual string getObjName(void) = 0;
        virtual UINT getObjID(void) = 0;
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(GLfloat delta) = 0;
        virtual void draw(int drawStyle,
                BOOL enableTexture,
                BOOL enablePYRRotation,
                BOOL enableMotion,
                GLfloat zStretch,
                GLfloat zOffset) = 0;

        virtual string genObjDesc(); // get the object descriptions (fixed part)
        virtual string genObjPara(); // get the object parameters (varied part)

        virtual string genObjDescTitle(); // get the object descriptions (fixed part)
        virtual string genObjParaTitle(); // get the object parameters (varied part)

        void reverseRotDirection();
        void rotate(GLfloat degree);

        void reset();

        enum enumProj2DMode {ORTHOGONAL, PROJECTION};
        enum enumRotDirection {CLOCKWISE, COUNTERCLOCKWISE};

        static const UINT objectID;

        //UINT texture; // FIX: how to represent texture here?
        
        enumProj2DMode proj2DMode;

        GLfloat pitch;
        GLfloat yaw;
        GLfloat roll;
        GLfloat height;
        GLfloat initZAsptRatio;
        GLfloat adjZAsptRatio;
        GLfloat rotSpeed;
        GLfloat maxRotDeg;

        GLfloat currRotDeg;
        enumRotDirection rotDirection;
      
        // Value range of the parameters
        rangeType<GLfloat> pitchRange;
        rangeType<GLfloat> yawRange;
        rangeType<GLfloat> rollRange;
        rangeType<GLfloat> heightRange;
        rangeType<GLfloat> initZAsptRatioRange;
        rangeType<GLfloat> rotSpeedRange;
        rangeType<GLfloat> maxRotDegRange;

        vector<texture_t *> textures;
    protected:
        string objName;
};
