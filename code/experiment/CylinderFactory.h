#pragma once
#include "testobjectfactory.h"
#include "CylinderObject.h"

class CylinderFactory :
    public TestObjectFactory
{
    public:
        CylinderFactory(void);
        virtual ~CylinderFactory(void);

        virtual TestObject* createObject(condCons_t& constraint);
        virtual const string getProductName();

    private:
        string productName;
};
