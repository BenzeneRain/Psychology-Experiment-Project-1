#include "stdafx.h"
#include "MixedObjectViewScene.h"
#include "Experiment.h"

#include <stdlib.h>

MixedObjectViewScene::MixedObjectViewScene(void)
{
}

MixedObjectViewScene::~MixedObjectViewScene(void)
{
}

TestObject *MixedObjectViewScene::getRandObj()
{
    Experiment *pExperi = Experiment::getInstance(NULL);
    int objPoolSize = pExperi->stubObjects.size();
    int randIndex = rand() % objPoolSize;
    TestObject *pTestObject = pExperi->stubObjects[randIndex];

    return pTestObject->newObj(); 
}

