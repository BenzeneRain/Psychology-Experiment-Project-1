#pragma once

#include "ConditionConstraints.h"
#include "TestObject.h"

class TestObjectFactory
{
    public:
        TestObjectFactory(void);
        virtual ~TestObjectFactory(void);

        virtual TestObject* createObject(condCons_t& constraint) = 0;
        virtual string getProductName() = 0;

    protected:
        string productName;
};
