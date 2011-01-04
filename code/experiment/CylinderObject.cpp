#include "stdafx.h"
#include "CylinderObject.h"
#include <string>
using namespace std;

CylinderObject::CylinderObject(void)
{
    this->objName = string("Cylinder");
}

CylinderObject::~CylinderObject(void)
{
}
        
TestObject *CylinderObject::newObj(void)
{
   //return dynamic_cast<TestObject *>(new CylinderObject()); 
   return (new CylinderObject()); 
}

string CylinderObject::getObjName(void)
{
    return this->objName;
}
