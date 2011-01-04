#pragma once
#include "scene.h"
#include "TestObject.h"

class MixedObjectViewScene :
    public Scene
{
    public:
        MixedObjectViewScene(void);
        virtual ~MixedObjectViewScene(void);
    
    protected:
        TestObject *getRandObj(void);
        TestObject *pObj;
};
