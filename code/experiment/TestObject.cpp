#include "stdafx.h"
#include "TestObject.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const UINT TestObject::objectID = 0;

TestObject::TestObject(vector<GLfloat>& pitchRange,
                    vector<GLfloat>& yawRange,
                    vector<GLfloat>& rollRange,
                    vector<GLfloat>& heightRange,
                    vector<GLfloat>& initZAsptRatioRange,
                    vector<GLfloat>& rotSpeedRange,
                    vector<GLfloat>& maxRotDegRange,
                    vector<texture_t *>& texs)
{
    this->textures = texs;
    this->pitchRange = pitchRange;
    this->yawRange = yawRange;
    this->rollRange = rollRange;
    this->heightRange = heightRange;
    this->initZAsptRatioRange = initZAsptRatioRange;
    this->rotSpeedRange = rotSpeedRange;
    this->maxRotDegRange = maxRotDegRange;

    this->currRotDeg = 0;
    this->adjZAsptRatio = 1.0f;
    this->rotDirection = CLOCKWISE;
}

TestObject::TestObject(TestObject& rObj)
{
    this->pitch = rObj.pitch;
    this->yaw = rObj.yaw;
    this->roll = rObj.roll;

    this->height = rObj.height;
    this->initZAsptRatio = rObj.initZAsptRatio;
    this->adjZAsptRatio = rObj.adjZAsptRatio;
    this->rotSpeed = rObj.rotSpeed;
    this->maxRotDeg = rObj.maxRotDeg;

    this->proj2DMode = rObj.proj2DMode;

    this->pitchRange = rObj.pitchRange;
    this->yawRange = rObj.yawRange;
    this->rollRange = rObj.rollRange;
    this->heightRange = rObj.heightRange;
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

    randIndex = rand() % this->pitchRange.size();
    this->pitch = this->pitchRange[randIndex];

    randIndex = rand() % this->yawRange.size();
    this->yaw = this->yawRange[randIndex];

    randIndex = rand() % this->rollRange.size();
    this->roll = this->rollRange[randIndex];

    randIndex = rand() % this->heightRange.size();
    this->height = this->heightRange[randIndex];

    randIndex = rand() % this->initZAsptRatioRange.size();
    this->initZAsptRatio = this->initZAsptRatioRange[randIndex];

    randIndex = rand() % this->rotSpeedRange.size();
    this->rotSpeed = this->rotSpeedRange[randIndex];

    randIndex = rand() % this->maxRotDegRange.size();
    this->maxRotDeg = this->maxRotDegRange[randIndex];

    //randIndex = rand() % this->initZAsptRatioRange.size();
    //this->adjZAsptRatio = this->initZAsptRatioRange[randIndex];
}

string TestObject::genObjDesc()
{
    ostringstream ossObj;

    ossObj << this->getObjID() << " "; // Object ID
    ossObj << this->getObjName() << " "; // Object type

    // Pitch range
    ossObj << this->pitchRange.size();
    for(unsigned int i = 0; i < this->pitchRange.size(); i ++)
    {
        ossObj << " " << this->pitchRange[i];
    }
    ossObj << " ";

    // Yaw Range
    ossObj << this->yawRange.size();
    for(unsigned int i = 0; i < this->yawRange.size(); i ++)
    {
        ossObj << " " << this->yawRange[i];
    }
    ossObj << " ";

    // Roll Range
    ossObj << this->rollRange.size();
    for(unsigned int i = 0; i < this->rollRange.size(); i ++)
    {
        ossObj << " " << this->rollRange[i];
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
    ossObj << this->pitch << " "; // pitch
    ossObj << this->yaw << " "; // yaw
    ossObj << this->roll << " "; // roll
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
