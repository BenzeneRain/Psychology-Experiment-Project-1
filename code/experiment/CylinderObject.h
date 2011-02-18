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
        CylinderObject(rangeType<GLfloat>& pitchRange,
                rangeType<GLfloat>& yawRange,
                rangeType<GLfloat>& rollRange,
                rangeType<GLfloat>& heightRange,
                rangeType<GLfloat>& initZAsptRatioRange,
                rangeType<GLfloat>& rotSpeedRange,
                rangeType<GLfloat>& maxRotDegRange,
                enumRotDirection defaultRotDirection,
                vector<texture_t *>& texs,
                rangeType<GLfloat>& radiusRange);
        CylinderObject(CylinderObject &rObj);
        virtual ~CylinderObject(void);

        virtual TestObject * newObj(TestObject &rObject);

        virtual string getObjName(void);
        virtual UINT getObjID(void);
        virtual void setRandPara(void);
        virtual BOOL adjustAsptRatio(GLfloat delta);
        virtual void draw(int drawStyle,
                BOOL enableTexture,
                BOOL enablePYRRotation,
                BOOL enableMotion,
                GLfloat xStretch,
                GLfloat yStretch,
                GLfloat zStretch,
                GLfloat xOffset,
                GLfloat yOffset,
                GLfloat zOffset);
        virtual void draw2D(int drawStyle,
                GLfloat xStretch,
                GLfloat yStretch,
                GLfloat zStretch,
                GLfloat xOffset,
                GLfloat yOffset,
                GLfloat zOffset);

        virtual string genObjDesc(void); // get the object descriptions (fixed part)
        virtual string genObjPara(void); // get the object parameters (varied part)

        virtual string genObjDescTitle(void); // get the object descriptions (fixed part)
        virtual string genObjParaTitle(void); // get the object parameters (varied part)

        virtual void buildModel(void);

        GLfloat radius;
        static const UINT objectID;

        GLuint topTextureID;
        GLuint bottomTextureID;
        GLuint sideTextureID;

        // value range of the parameters
        rangeType<GLfloat> radiusRange;
    private:
        string objName;

        vector<GLfloat> cylinderBodyCoords;
        vector<UINT> cylinderBodyCoordIndices;
        vector<GLfloat> cylinderBodyTexCoords;

        void _drawCylinderBody(bool enableTexture);
        void _modelCylinderBody(int slices);
};
