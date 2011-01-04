#include "stdafx.h"
#include "CylinderObject.h"
#include <string>
using namespace std;

const UINT CylinderObject::objectID = 0;

CylinderObject::CylinderObject(void)
{
    this->objName = string("Cylinder");
}

CylinderObject::~CylinderObject(void)
{
}
        
TestObject *CylinderObject::newObj(void)
{
   return (new CylinderObject()); 
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
}
