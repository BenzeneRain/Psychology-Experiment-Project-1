#include "stdafx.h"
#include "TestObject.h"
#include <vector>
using namespace std;

const UINT TestObject::objectID = 0;

TestObject::TestObject(void)
{
    // Initialize the ranges
    // FIX: The value should be read from
    // the configuration files later
    slantRange.push_back(0.0f);

    heightRange.push_back(0.1f);
    heightRange.push_back(0.5f);
    heightRange.push_back(0.9f);
    
    tiltRange.push_back(0.0f);

    initZAsptRatioRange.push_back(0.8f);
    initZAsptRatioRange.push_back(1.0f);
    initZAsptRatioRange.push_back(1.2f);

    rotSpeedRange.push_back(1.0f);
    rotSpeedRange.push_back(5.0f);
    rotSpeedRange.push_back(10.0f);

    maxRotDegRange.push_back(60.0f);
    maxRotDegRange.push_back(90.0f);
    maxRotDegRange.push_back(120.0f);
}

TestObject::TestObject(TestObject& rObj)
{
   this->slant = rObj.slant;

   this->height = rObj.height;
   this->tilt = rObj.tilt;
   this->initZAsptRatio = rObj.initZAsptRatio;
   this->adjZAsptRatio = rObj.adjZAsptRatio;
   this->rotSpeed = rObj.rotSpeed;
   this->maxRotDeg = rObj.maxRotDeg;

   this->proj2DMode = rObj.proj2DMode;

   this->slantRange = rObj.slantRange;
   this->heightRange = rObj.heightRange;
   this->tiltRange = rObj.tiltRange;
   this->initZAsptRatioRange = rObj.initZAsptRatioRange;
   this->rotSpeedRange = rObj.rotSpeedRange;
   this->maxRotDegRange = rObj.maxRotDegRange;
}

TestObject::~TestObject(void)
{
}

void TestObject::setRandPara()
{
    int randIndex;

    // FIX: rand() maybe is not good enough

    randIndex = rand() % this->slantRange.size();
    this->slant = this->slantRange[randIndex];

    randIndex = rand() % this->heightRange.size();
    this->height = this->heightRange[randIndex];

    randIndex = rand() % this->tiltRange.size();
    this->tilt = this->tiltRange[randIndex];

    randIndex = rand() % this->initZAsptRatioRange.size();
    this->initZAsptRatio = this->initZAsptRatioRange[randIndex];

    randIndex = rand() % this->rotSpeedRange.size();
    this->rotSpeed = this->rotSpeedRange[randIndex];

    randIndex = rand() % this->maxRotDegRange.size();
    this->maxRotDeg = this->maxRotDegRange[randIndex];

    // Set adjAsptRatio
    this->adjZAsptRatio = this->initZAsptRatio;
}
