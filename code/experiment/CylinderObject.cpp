#include "stdafx.h"
#include "TestObject.h"
#include "CylinderObject.h"
#include <string>
#include <sstream>
#include <string>
using namespace std;

const UINT CylinderObject::objectID = 1;

CylinderObject::CylinderObject(void) : TestObject()
{
    this->objName = string("Cylinder");

    // Initialize the ranges
    // FIX: The value should be read from
    // the configuration files later
    radiusRange.push_back(20.0f);
    radiusRange.push_back(30.0f);
    radiusRange.push_back(40.0f);
}

CylinderObject::CylinderObject(vector<GLfloat>& slantRange,
                            vector<GLfloat>& heightRange,
                            vector<GLfloat>& tiltRange,
                            vector<GLfloat>& initZAsptRatioRange,
                            vector<GLfloat>& rotSpeedRange,
                            vector<GLfloat>& maxRotDegRange,
                            vector<GLfloat>& radiusRange,
                            vector<GLuint>& textureID):
    TestObject(slantRange, heightRange, tiltRange, initZAsptRatioRange,
            rotSpeedRange, maxRotDegRange) 
{
    this->radiusRange = radiusRange;    
    this->topTextureID = textureID[0];
    this->bottomTextureID = textureID[1];
    this->sideTextureID = textureID[2];

    this->objName = string("Cylinder");
}

CylinderObject::CylinderObject(CylinderObject &rObj) : TestObject(rObj)
{
    this->objName = string("Cylinder");
    this->radius = rObj.radius;
}

CylinderObject::~CylinderObject(void)
{
}
        
TestObject *CylinderObject::newObj(void)
{
   return (new CylinderObject()); 
}

TestObject *CylinderObject::newObj(TestObject &rObject)
{
   return (new CylinderObject(static_cast< CylinderObject& >(rObject))); 
}

string CylinderObject::getObjName(void)
{
    return this->objName;
}

UINT CylinderObject::getObjID()
{
    return this->objectID;
}

void CylinderObject::setRandPara()
{
    TestObject::setRandPara();

    int randIndex;

    // FIX: rand() maybe is not good enough

    randIndex = rand() % this->radiusRange.size();
    this->radius = this->radiusRange[randIndex];
}

BOOL CylinderObject::adjustAsptRatio(GLfloat delta)
{
    if(adjZAsptRatio + delta <= 0)
    {
        return FALSE;
    }
    else
    {
        adjZAsptRatio += delta;
        return TRUE;
    }
}

string CylinderObject::genObjDesc()
{

    ostringstream ossObj;
    ossObj << TestObject::genObjDesc();

    // Radius Range
    ossObj << this->radiusRange.size();
    for(unsigned int i = 0; i < this->radiusRange.size(); i ++)
    {
        ossObj << " " << this->radiusRange[i];
    }
    ossObj << " ";

    return ossObj.str();
}

string CylinderObject::genObjPara()
{
    string strPara;

    strPara = TestObject::genObjPara();

    ostringstream ossObj;

    ossObj << this->radius << " "; // Radius of the Cylinder

    strPara += ossObj.str();

    return strPara;
}

void CylinderObject::draw(int drawStyle, BOOL enableTexture)
{
    GLUquadricObj *pCylinder;

    if(enableTexture)
    {
        glEnable(GL_TEXTURE_2D);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    {
        glPushMatrix();
        glRotatef(90, -1.0f, 0.0f, 0.0f);

        if(enableTexture)
        {
            glBindTexture(GL_TEXTURE_2D, this->sideTextureID);
        }
        pCylinder = gluNewQuadric();
        gluQuadricDrawStyle(pCylinder, drawStyle);
        gluQuadricNormals(pCylinder, GLU_SMOOTH);
        gluQuadricTexture(pCylinder, GL_TRUE);

        // Draw the cylinder
        gluCylinder(pCylinder, this->radius, this->radius, this->height, 32, 32);

        gluDeleteQuadric(pCylinder);

        // Draw the bottom face
        if(enableTexture)
        {
            glBindTexture(GL_TEXTURE_2D, this->bottomTextureID);
        }
        pCylinder = gluNewQuadric();
        gluQuadricDrawStyle(pCylinder, drawStyle);
        gluQuadricNormals(pCylinder, GLU_SMOOTH);
        gluQuadricTexture(pCylinder, GL_TRUE);

        gluDisk(pCylinder, 0.0f, this->radius, 32, 1);
        gluDeleteQuadric(pCylinder);

        {
            // Draw the top face
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, this->height);

            if(enableTexture)
            {
                glBindTexture(GL_TEXTURE_2D, this->topTextureID);
            }
            pCylinder = gluNewQuadric();
            gluQuadricDrawStyle(pCylinder, drawStyle);
            gluQuadricNormals(pCylinder, GLU_SMOOTH);
            gluQuadricTexture(pCylinder, GL_TRUE);

            gluDisk(pCylinder, 0.0f, this->radius, 32, 1);
            gluDeleteQuadric(pCylinder);

            glPopMatrix();
        }

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}
