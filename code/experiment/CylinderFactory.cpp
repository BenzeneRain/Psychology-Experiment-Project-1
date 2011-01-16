#include "stdafx.h"
#include "CylinderFactory.h"

CylinderFactory::CylinderFactory(void)
{
    this->productName = "Cylinder";
}

CylinderFactory::~CylinderFactory(void)
{
}

TestObject* CylinderFactory::createObject(condCons_t& constraint, vector<texture_t *>& textures)
{
    // Check if the number of textures is larger than 3
    if(textures.size() < 3)
    {
        ostringstream ossWarning;
        ossWarning << "The Cylinder requires three textures, but only " 
            << textures.size() << " provided.";
        
        string warningMsg = ossWarning.str();
        MessageBox(NULL, (LPCSTR)(warningMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
        return NULL;
    }

    TestObject *pObj = new CylinderObject(constraint.slantRange, 
            constraint.heightRange,
            constraint.tiltRange,
            constraint.initZAsptRatioRange,
            constraint.rotSpeedRange,
            constraint.maxRotDegRange,
            textures,
            constraint.radiusRange);

    return pObj;
}

const string CylinderFactory::getProductName()
{
    return this->productName;
}
