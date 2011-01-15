#include "stdafx.h"
#include "TestObject.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const UINT TestObject::objectID = 0;

TestObject::TestObject(void)
{
    // Initialize the ranges
    // FIX: The value should be read from
    // the configuration files later
    this->slantRange.push_back(0.0f);

    this->heightRange.push_back(60.0f);
    this->heightRange.push_back(70.0f);
    this->heightRange.push_back(80.0f);

    this->tiltRange.push_back(0.0f);

    this->initZAsptRatioRange.push_back(0.8f);
    this->initZAsptRatioRange.push_back(1.0f);
    this->initZAsptRatioRange.push_back(1.2f);

    this->rotSpeedRange.push_back(20.0f);
    this->rotSpeedRange.push_back(25.0f);
    this->rotSpeedRange.push_back(40.0f);

    this->maxRotDegRange.push_back(20.0f);
    this->maxRotDegRange.push_back(60.0f);
    this->maxRotDegRange.push_back(90.0f);
    this->maxRotDegRange.push_back(120.0f);

    this->currRotDeg = 0;
    this->rotDirection = CLOCKWISE;
}

TestObject::TestObject(vector<GLfloat>& slantRange,
                    vector<GLfloat>& heightRange,
                    vector<GLfloat>& tiltRange,
                    vector<GLfloat>& initZAsptRatioRange,
                    vector<GLfloat>& rotSpeedRange,
                    vector<GLfloat>& maxRotDegRange)
{
    this->slantRange = slantRange;
    this->heightRange = heightRange;
    this->tiltRange = tiltRange;
    this->initZAsptRatioRange = initZAsptRatioRange;
    this->rotSpeedRange = rotSpeedRange;
    this->maxRotDegRange = maxRotDegRange;

    this->currRotDeg = 0;
    this->rotDirection = CLOCKWISE;
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

    // FIX: rand() maybe not good enough

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

    randIndex = rand() % this->initZAsptRatioRange.size();
    this->adjZAsptRatio = this->initZAsptRatioRange[randIndex];
}

string TestObject::genObjDesc()
{
    ostringstream ossObj;

    ossObj << this->getObjID() << " "; // Object ID
    ossObj << this->getObjName() << " "; // Object type

    // Slant range
    ossObj << this->slantRange.size();
    for(unsigned int i = 0; i < this->slantRange.size(); i ++)
    {
        ossObj << " " << this->slantRange[i];
    }
    ossObj << " ";

    // Tilt Range
    ossObj << this->tiltRange.size();
    for(unsigned int i = 0; i < this->tiltRange.size(); i ++)
    {
        ossObj << " " << this->tiltRange[i];
    }
    ossObj << " ";
    
    // Height Range
    ossObj << this->heightRange.size();
    for(unsigned int i = 0; i < this->heightRange.size(); i ++)
    {
        ossObj << " " << this->heightRange[i];
    }
    ossObj << " ";

    // Initial Aspect Ratio on Z-axis range
    ossObj << this->initZAsptRatioRange.size();
    for(unsigned int i = 0; i < this->initZAsptRatioRange.size(); i ++) 
    {
        ossObj << " " << this->initZAsptRatioRange[i];
    }
    ossObj << " ";

    // Object Rotation Speed Range
    ossObj << this->rotSpeedRange.size();
    for(unsigned int i = 0; i < this->rotSpeedRange.size(); i ++) 
    {
        ossObj << " " << this->rotSpeedRange[i];
    }
    ossObj << " ";

    // Object maximum rotation degree Range
    ossObj << this->maxRotDegRange.size();
    for(unsigned int i = 0; i < this->maxRotDegRange.size(); i ++)
    {
        ossObj << " " << this->maxRotDegRange[i];
    }
    ossObj << " ";

    return ossObj.str();
}

string TestObject::genObjPara()
{
    ostringstream ossObj;

    ossObj << this->getObjID() << " "; // Object ID
    ossObj << this->getObjName() << " "; // Object type
    ossObj << this->slant << " "; // slant
    ossObj << this->tilt << " "; // tilt
    ossObj << this->height << " "; // height
    ossObj << this->initZAsptRatio << " "; // Object initial Aspect Ratio on Z axis
    ossObj << this->adjZAsptRatio << " "; // Object aspect ratio on Z-axis after subject adjusted
    ossObj << this->rotSpeed << " "; // Object Rotation Speed
    ossObj << this->maxRotDeg << " "; // Object Maximum Rotation Degree

    return ossObj.str();
}

void TestObject::reverseRotDirection()
{
    if(this->rotDirection == CLOCKWISE)
        this->rotDirection = COUNTERCLOCKWISE;
    else
        this->rotDirection = CLOCKWISE;
}
