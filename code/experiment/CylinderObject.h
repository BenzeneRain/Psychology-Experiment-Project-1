#pragma once
#include "testobject.h"
#include <string>

using namespace std;

class CylinderObject :
    public TestObject
{
    public:
        CylinderObject(void);
        virtual ~CylinderObject(void);

        virtual TestObject * newObj(void);
        virtual string getObjName(void);

    private:
        string objName;
};
