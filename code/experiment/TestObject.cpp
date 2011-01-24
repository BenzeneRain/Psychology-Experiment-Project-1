#include "stdafx.h"
#include "TestObject.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const UINT TestObject::objectID = 0;

TestObject::TestObject(rangeType<GLfloat>& pitchRange,
                    rangeType<GLfloat>& yawRange,
                    rangeType<GLfloat>& rollRange,
                    rangeType<GLfloat>& heightRange,
                    rangeType<GLfloat>& initZAsptRatioRange,
                    rangeType<GLfloat>& rotSpeedRange,
                    rangeType<GLfloat>& maxRotDegRange,
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
    this->pitch = this->pitchRange.getRandomValue();

    this->yaw = this->yawRange.getRandomValue();
    this->roll = this->rollRange.getRandomValue();
    this->height = this->heightRange.getRandomValue();

    this->initZAsptRatio = this->initZAsptRatioRange.getRandomValue();

    this->rotSpeed = this->rotSpeedRange.getRandomValue();
    this->maxRotDeg = this->maxRotDegRange.getRandomValue();

    //this->adjZAsptRatio = this->initZAsptRatioRange.getRandomValue();
}

string TestObject::genObjDescTitle()
{
    ostringstream ossObj;

    ossObj << "ObjectID" << " | "; // Object ID
    //ossObj << "ObjectName" << " | "; // Object type

    // Pitch range
    ossObj << "Pitch Range" << " | ";

    // Yaw Range
    ossObj << "Yaw Range" << " | ";

    // Roll Range
    ossObj << "Roll Range" << " | ";

    // Height Range
    ossObj << "Height Range" << " | ";

    // Initial Aspect Ratio on Z-axis range
    ossObj << "Initial Aspect Ratio on Z-axis range" << " | ";

    // Object Rotation Speed Range
    ossObj << "Rotation Speed Range" << " | ";

    // Object maximum rotation degree Range
    ossObj << "Max Rotation Degree Range" << " | ";

    return ossObj.str();
}

string TestObject::genObjDesc()
{
    ostringstream ossObj;

    ossObj << this->getObjID() << "\t"; // Object ID
    //ossObj << this->getObjName() << "\t"; // Object type

    // Pitch range
    ossObj << this->pitchRange << "\b\t";

    // Yaw Range
    ossObj << this->yawRange << "\b\t";

    // Roll Range
    ossObj << this->rollRange << "\b\t";

    // Height Range
    ossObj << this->heightRange << "\b\t";

    // Initial Aspect Ratio on Z-axis range
    ossObj << this->initZAsptRatioRange << "\b\t";

    // Object Rotation Speed Range
    ossObj << this->rotSpeedRange << "\b\t";

    // Object maximum rotation degree Range
    ossObj << this->maxRotDegRange << "\b\t";

    return ossObj.str();
}

string TestObject::genObjParaTitle()
{
    ostringstream ossObj;

    ossObj << "Object ID" << "\t| "; // Object ID
    //ossObj << "Object name" << " | "; // Object type
    ossObj << "Pitch" << "\t| "; // pitch
    ossObj << "Yaw"<< "\t| "; // yaw
    ossObj << "Roll" << "\t| "; // roll
    ossObj << "Height"<< "\t| "; // height
    ossObj << "Initial aspect ratio on Z" << "\t| "; // Object initial Aspect Ratio on Z axis
    ossObj << "Adjusted aspect ratio on Z"<< "\t| "; // Object aspect ratio on Z-axis after subject adjusted
    ossObj << "Rotation speed" << "\t| "; // Object Rotation Speed
    ossObj << "Max Rotation Degree" << "\t| "; // Object Maximum Rotation Degree

    return ossObj.str();
}

string TestObject::genObjPara()
{
    ostringstream ossObj;
    ossObj.precision(3);
    ossObj << this->getObjID() << "\t\t  "; // Object ID
    //ossObj << this->getObjName() << "\t"; // Object type
    ossObj << this->pitch << "\t  "; // pitch
    ossObj << this->yaw << "\t  "; // yaw
    ossObj << this->roll << "\t  "; // roll
    ossObj << this->height << "\t\t  "; // height
    ossObj << this->initZAsptRatio << "\t\t\t\t  "; // Object initial Aspect Ratio on Z axis
    ossObj << this->adjZAsptRatio << "\t\t\t\t  "; // Object aspect ratio on Z-axis after subject adjusted
    ossObj << this->rotSpeed << "\t\t\t  "; // Object Rotation Speed
    ossObj << this->maxRotDeg << "\t\t\t  "; // Object Maximum Rotation Degree

    return ossObj.str();
}

void TestObject::reverseRotDirection()
{
    if(this->rotDirection == CLOCKWISE)
        this->rotDirection = COUNTERCLOCKWISE;
    else
        this->rotDirection = CLOCKWISE;
}

void TestObject::rotate(GLfloat degree)
{
    if((this->rotDirection == TestObject::CLOCKWISE &&
                this->currRotDeg - degree < -(this->maxRotDeg)))
    {
        GLfloat degDist = -this->maxRotDeg - (this->currRotDeg - degree);
        this->currRotDeg = -this->maxRotDeg + degDist; 
        this->reverseRotDirection(); 
    }
    else if(this->rotDirection == TestObject::CLOCKWISE)
    {
        this->currRotDeg -= degree;
    }
    else if((this->rotDirection == TestObject::COUNTERCLOCKWISE && 
                this->currRotDeg + degree > this->maxRotDeg))
    {
        GLfloat degDist = this->currRotDeg + degree - this->maxRotDeg;
        this->currRotDeg = this->maxRotDeg - degDist;
        this->reverseRotDirection(); 
    }
    else
    {
        this->currRotDeg += degree;
    }
}

void TestObject::reset()
{
    this->currRotDeg = 0;
    this->adjZAsptRatio = 1.0f;
    this->rotDirection = CLOCKWISE;
}
