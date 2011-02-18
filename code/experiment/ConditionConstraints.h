#pragma once
#include "stdafx.h"
#include "rangeType.h"

enum DisplayMode {CONTINUOUS_DISPLAY, DISCRETE_DISPLAY};
typedef enum DisplayMode displayMode_t;

class timeStructForDiscreteCase
{
public:
    timeStructForDiscreteCase(float display, float disappear):
      secDisplay(display), secBlackScreen(disappear){}
    // Used only when the display mode is DISCRETE_DISPLAY
    float secDisplay; // seconds for displaying object
    float secBlackScreen; // seconds for displaying black screen

    friend ostream& operator<<(ostream& os, timeStructForDiscreteCase& rtime)
    {
        os << rtime.secDisplay << " " << rtime.secBlackScreen;
        return os;
    }
};
typedef class timeStructForDiscreteCase timeStruct_t;

struct conditionConstraints
{
    vector<string> objectNames;

    rangeType<GLfloat> radiusRange;
    rangeType<GLfloat> pitchRange;
    rangeType<GLfloat> yawRange;
    rangeType<GLfloat> rollRange;

    rangeType<GLfloat> heightRange;
    rangeType<GLfloat> initZAsptRatioRange;
    rangeType<GLfloat> rotSpeedRange;
    rangeType<GLfloat> maxRotDegRange;

    // Used only when the display mode is DISCRETE_DISPLAY
    rangeType<timeStruct_t> time;

    vector< vector<string> * > textureGroups;

    int weight; // The weight of the constraint

    displayMode_t dispMode;

    int groupID;
    int id;

    string genDescTitle()
    {
        ostringstream ossTitle;

        ossTitle << "Condition Group ID\t| Condition ID\t| Motion\t| "
            << "Object Display Duration\t| Object Disappear Duration\t| Object Names\t| Pitch Range\t| Yaw Range\t| "
            << "Roll Range\t| Height Range\t| Initial Aspect Ration Range on Z\t| Rotation Speed Range\t| "
            << "Max Rotation Degree Range\t| Texture Groups\t|" << endl;

        return ossTitle.str();
    }

    string genDesc()
    {
        ostringstream ossDesc;
        
        ossDesc << this->groupID << "\t\t\t  " << this->id << "\t\t  ";

        if(this->dispMode == CONTINUOUS_DISPLAY)
            ossDesc << "1";
        else
            ossDesc << "0";
        ossDesc << "\t\t  ";

        ossDesc.precision(2);
        ossDesc << this->time << "\t\t ";

        ossDesc << this->objectNames.size();
        for(unsigned int i = 0; i < this->objectNames.size(); i ++)
            ossDesc << " " << this->objectNames[i];
        ossDesc << "\t  ";
        

        ossDesc << this->pitchRange << "\t  ";
        ossDesc << this->yawRange << "\t  ";
        ossDesc << this->rollRange << "\t  ";
        ossDesc << this->heightRange << "\t  ";
        ossDesc << this->initZAsptRatioRange << "\t\t\t\t  ";
        ossDesc << this->rotSpeedRange << "\t\t  ";
        ossDesc << this->maxRotDegRange << "\t\t\t  ";

        ossDesc << this->textureGroups.size() << " ";
        for(unsigned int i = 0; i < this->textureGroups.size(); i ++)
        {
            ossDesc << this->textureGroups[i]->size();
            for(unsigned int j = 0; j < this->textureGroups[i]->size(); j ++)
            {
                ossDesc << " " << (*this->textureGroups[i])[j];
            }
            ossDesc << "\t\t";
        }

        return ossDesc.str();
    }
};
typedef struct conditionConstraints condCons_t;
