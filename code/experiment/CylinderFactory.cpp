#include "stdafx.h"
#include "CylinderFactory.h"

CylinderFactory::CylinderFactory(void)
{
    this->productName = "Cylinder";
}

CylinderFactory::~CylinderFactory(void)
{
}

TestObject* CylinderFactory::createObject(condCons_t& constraint, vector<GLuint>& textureIDs)
{
    // TODO: Check if the number of textures is larger than 3

    TestObject *pObj = new CylinderObject(constraint.slantRange, 
            constraint.heightRange,
            constraint.tiltRange,
            constraint.initZAsptRatioRange,
            constraint.rotSpeedRange,
            constraint.maxRotDegRange,
            constraint.radiusRange,
            textureIDs);

    return pObj;
}

const string CylinderFactory::getProductName()
{
    return this->productName;
}
