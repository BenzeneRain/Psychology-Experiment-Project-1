#include "stdafx.h"
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
                            vector<GLfloat>& radiusRange):
    TestObject(slantRange, heightRange, tiltRange, initZAsptRatioRange,
            rotSpeedRange, maxRotDegRange) 
{
    this->radiusRange = radiusRange;    
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
    string strDesc;

    strDesc = TestObject::genObjDesc();

    ostringstream ossObj;

    ossObj << "Radius range: ";
    for(vector<GLfloat>::iterator it = this->radiusRange.begin(); 
        it != this->radiusRange.end(); it ++)
        {
            ossObj << (GLfloat)(*it) << " ";
        }
    ossObj << endl;

    strDesc += ossObj.str();

    return strDesc;
}

string CylinderObject::genObjPara()
{
    string strPara;

    strPara = TestObject::genObjPara();

    ostringstream ossObj;

    ossObj << "Radius: " << this->radius << endl;

    strPara += ossObj.str();

    return strPara;
}

void CylinderObject::draw(int drawStyle)
{
    GLUquadricObj *pCylinder;

    glPushMatrix();

    glRotatef(90, -1.0f, 0.0f, 0.0f);

    pCylinder = gluNewQuadric();

    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    gluQuadricTexture(pCylinder, GL_TRUE);

    // Draw the cylinder
    gluCylinder(pCylinder, this->radius, this->radius, this->height, 32, 32);

    gluDeleteQuadric(pCylinder);

    // Draw the bottom face
    pCylinder = gluNewQuadric();

    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    gluQuadricTexture(pCylinder, GL_TRUE);

    gluDisk(pCylinder, 0.0f, this->radius, 1024, 2);
    gluDeleteQuadric(pCylinder);

    // Draw the top face

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->height);

    pCylinder = gluNewQuadric();

    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    gluQuadricTexture(pCylinder, GL_TRUE);

    gluDisk(pCylinder, 0.0f, this->radius, 1024, 2);
    gluDeleteQuadric(pCylinder);

    glPopMatrix();

    glPopMatrix();
}
