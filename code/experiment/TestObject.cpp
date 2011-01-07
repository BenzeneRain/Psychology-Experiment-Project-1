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
    slantRange.push_back(0.0f);

    heightRange.push_back(3.0f);
    heightRange.push_back(5.0f);
    heightRange.push_back(9.0f);
    
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

    // Set adjAsptRatio
    this->adjZAsptRatio = this->initZAsptRatio;
}

string TestObject::genObjDesc()
{
    ostringstream ossObj;

    ossObj << "Object ID: " << this->getObjID() << endl;
    ossObj << "Object type: " << this->getObjName() << endl;

    ossObj << "Slant range:";
    for(vector<GLfloat>::iterator it = this->slantRange.begin(); 
        it != this->slantRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    ossObj << "Height range:";
    for(vector<GLfloat>::iterator it = this->heightRange.begin(); 
        it != this->heightRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    ossObj << "Tilt range:";
    for(vector<GLfloat>::iterator it = this->tiltRange.begin(); 
        it != this->tiltRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    ossObj << "Initial aspect ratio on Z-axis range:";
    for(vector<GLfloat>::iterator it = this->initZAsptRatioRange.begin(); 
        it != this->initZAsptRatioRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    ossObj << "Object rotation speed range:";
    for(vector<GLfloat>::iterator it = this->rotSpeedRange.begin(); 
        it != this->rotSpeedRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    ossObj << "Object maximum rotation degree range:";
    for(vector<GLfloat>::iterator it = this->maxRotDegRange.begin(); 
        it != this->maxRotDegRange.end(); it ++)
        {
            ossObj << " " << (GLfloat)(*it);
        }
    ossObj << endl;

    return ossObj.str();
}

string TestObject::genObjPara()
{
    ostringstream ossObj;

    ossObj << "Object ID: " << this->getObjID() << endl;
    ossObj << "Object type: " << this->getObjName() << endl;
    ossObj << "Object slant: " << this->slant << endl;
    ossObj << "Object height: " << this->height << endl;
    ossObj << "Object tilt: " << this->tilt << endl;
    ossObj << "Object initial aspect ratio on Z-axis: " << this->initZAsptRatio << endl;
    ossObj << "Object aspect ratio on Z-axis after subject adjusted: " << this->adjZAsptRatio << endl;
    ossObj << "Object rotation speed: " << this->rotSpeed << endl;
    ossObj << "Object maximum rotation degree: " << this->maxRotDeg << endl;

    return ossObj.str();
}
