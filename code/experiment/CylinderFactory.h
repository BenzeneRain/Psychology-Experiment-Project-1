#pragma once
#include "stdafx.h"
#include "testobjectfactory.h"
#include "CylinderObject.h"

class CylinderFactory :
    public TestObjectFactory
{
    public:
        CylinderFactory(void);
        virtual ~CylinderFactory(void);

        virtual TestObject* createObject(condCons_t& constraint, vector<GLuint>& textureIDs);
        virtual const string getProductName();

    private:
        string productName;
};
