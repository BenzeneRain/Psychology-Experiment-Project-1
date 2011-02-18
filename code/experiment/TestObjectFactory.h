#pragma once

#include "stdafx.h"
#include "ConditionConstraints.h"
#include "texture.h"
#include "TestObject.h"

class TestObjectFactory
{
    public:
        TestObjectFactory(void);
        virtual ~TestObjectFactory(void);

        virtual TestObject* createObject(condCons_t& constraint, vector<texture_t *>& textures) = 0;
        virtual vector<TestObject *> createAllObjects(condCons_t& constraint, vector<texture_t *>& textures) = 0;
        virtual const string getProductName() = 0;

    protected:
        string productName;
};
