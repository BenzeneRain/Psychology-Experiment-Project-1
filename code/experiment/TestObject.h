#pragma once

#include <string>

using namespace std;

class TestObject
{
    public:
        TestObject(void);
        virtual ~TestObject(void);
    
        virtual TestObject * newObj(void) = 0;
        virtual string getObjName(void) = 0;

    protected:
        string objName;
};
