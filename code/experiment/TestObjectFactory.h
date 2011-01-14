#pragma once

#include "stdafx.h"
#include "ConditionConstraints.h"
#include "TestObject.h"

class TestObjectFactory
{
    public:
        TestObjectFactory(void);
        virtual ~TestObjectFactory(void);

        virtual TestObject* createObject(condCons_t& constraint, vector<GLuint>& textureIDs) = 0;
        virtual const string getProductName() = 0;

    protected:
        string productName;
};
