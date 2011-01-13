#pragma once
#include "testobjectfactory.h"
#include "CylinderObject.h"

class CylinderFactory :
    public TestObjectFactory
{
    public:
        CylinderFactory(void);
        virtual ~CylinderFactory(void);

        TestObject* createObject(condCons_t& constraint);

        virtual string getObjectName() = 0;

    private:
        string productName;
};
