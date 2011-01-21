#pragma once
#include "stdafx.h"
#include "rangeType.h"

enum DisplayMode {CONTINUOUS_DISPLAY, DISCRETE_DISPLAY};
typedef enum DisplayMode displayMode_t;

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

    vector< vector<string> * > textureGroups;

    int weight; // The weight of the constraint

    displayMode_t dispMode;
    // Used only when the display mode is DISCRETE_DISPLAY
    float secDisplay; // seconds for displaying object
    float secBlackScreen; // seconds for displaying black screen

    int groupID;
    int id;

    string genDescTitle()
    {
        ostringstream ossTitle;

        ossTitle << "Group ID | Constraint ID | Object Names | Pitch Range | Yaw Range | "
            << "Roll Range | Height Range | Initial Aspect Ration Range on Z | Rotation Speed Range |"
            << "Max Rotation Degree Range | Texture Groups |" << endl;

        return ossTitle.str();
    }

    string genDesc()
    {
        ostringstream ossDesc;
        
        ossDesc << this->groupID << "\t" << this->id << "\t";
        ossDesc << this->objectNames.size();
        for(unsigned int i = 0; i < this->objectNames.size(); i ++)
            ossDesc << " " << this->objectNames[i];
        ossDesc << "\t";

        ossDesc << this->pitchRange << "\t";
        ossDesc << this->yawRange << "\t";
        ossDesc << this->rollRange << "\t";
        ossDesc << this->heightRange << "\t";
        ossDesc << this->initZAsptRatioRange << "\t";
        ossDesc << this->rotSpeedRange << "\t";
        ossDesc << this->maxRotDegRange << "\t";

        ossDesc << this->textureGroups.size() << " ";
        for(unsigned int i = 0; i < this->textureGroups.size(); i ++)
        {
            ossDesc << this->textureGroups[i]->size();
            for(unsigned int j = 0; j < this->textureGroups[i]->size(); j ++)
            {
                ossDesc << " " << (*this->textureGroups[i])[j];
            }
            ossDesc << "\t";
        }

        return ossDesc.str();
    }
};
typedef struct conditionConstraints condCons_t;
