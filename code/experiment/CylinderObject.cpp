#include "stdafx.h"
#include "CylinderObject.h"
#include <string>
using namespace std;

const UINT CylinderObject::objectID = 1;

CylinderObject::CylinderObject(void) : TestObject()
{
    this->objName = string("Cylinder");

    // Initialize the ranges
    // FIX: The value should be read from
    // the configuration files later
    radiusRange.push_back(0.3f);
    radiusRange.push_back(0.6f);
    radiusRange.push_back(0.9f);
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