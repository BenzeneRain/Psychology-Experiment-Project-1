#pragma once
#include "stdafx.h"
#include "testobjectfactory.h"
#include "texture.h"
#include "CylinderObject.h"

class CylinderFactory :
    public TestObjectFactory
{
    public:
        CylinderFactory(void);
        virtual ~CylinderFactory(void);

        virtual TestObject* createObject(condCons_t& constraint, vector<texture_t *>& textures);
        virtual const string getProductName();

    private:
        string productName;
};
