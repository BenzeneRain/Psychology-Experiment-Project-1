#include "StdAfx.h"
#include "gBConditionsOneSecOneGroup.h"

gBConditionsOneSecOneGroup::gBConditionsOneSecOneGroup(ifstream& rFin, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    groupBasedConditions(rFin, numConditions, rObjectFactories, rScr)
{
    this->currSection = 0;
    this->currGroupID = -1;
}

gBConditionsOneSecOneGroup::gBConditionsOneSecOneGroup(ifstream& rFin,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    groupBasedConditions(rFin, rObjectFactories, rScr)
{
    this->currSection = 0;
    this->currGroupID = -1;
}

gBConditionsOneSecOneGroup::~gBConditionsOneSecOneGroup()
{
}

BOOL gBConditionsOneSecOneGroup::initConditions()
{

    BOOL ret = groupBasedConditions::initConditions();

    // Read Repeat times for the group
    ifstream& fin = this->overallFin;
    string junk;
    int repeatTimes;

    for(int i = 0; i < numGroups; i ++)
    {
        fin >> repeatTimes;
        if(repeatTimes <= 0)
        {
            ostringstream ossError;
            ossError << "Invalid group repeat section times " << repeatTimes
                << " for group " << i + 1;
            string errorMsg = ossError.str();
            MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);

            return FALSE;
        }

        this->_groupRepeatTimes.push_back(repeatTimes);
    }
    getline(fin, junk);

    if(ret != FALSE)
    {
        // Generate conditions according to constraints
        ret = this->generateAllConditions();
        if(ret != FALSE)
        {
            this->_shuffleGroups(1023);   
        }
    }

    return ret;
}

void gBConditionsOneSecOneGroup::shuffleConditions(int times)
{
    this->_updateCurrentGroupID();

    for(int i = 0; i < times; i ++)
        random_shuffle(this->_currConditions.begin(), this->_currConditions.end());    
}

cond_t& gBConditionsOneSecOneGroup::operator[](int &rhs)
{
    this->_updateCurrentGroupID();
    return *this->_currConditions[rhs];
}

const vector<cond_t *>& gBConditionsOneSecOneGroup::getAllConditions()
{
    this->_updateCurrentGroupID();
    return this->_currConditions;
}

BOOL gBConditionsOneSecOneGroup::clearConditions()
{
    BOOL ret;
    ret = groupBasedConditions::clearConditions();

    vector<cond_t *>().swap(this->_currConditions);

    this->currSection = 0;
    this->currGroupID = -1;

    return ret;
}

void gBConditionsOneSecOneGroup::_updateCurrentGroupID()
{
    int groupIndex = 0;

    vector<int>::iterator indexIter = lower_bound(this->_sectionBounds.begin(),
            this->_sectionBounds.end(), this->currSection);

    groupIndex = int(indexIter - this->_sectionBounds.begin());

    if(this->currGroupID != groupIndex)
    {
        this->currGroupID = groupIndex;
        vector<cond_t *>().swap(this->_currConditions);

        for(int i = 0; i <(int) this->conditionGroups[this->currGroupID]->size(); i ++)
        {
            this->_currConditions.push_back(this->conditions[(*this->conditionGroups[this->currGroupID])[i]]);
        }
    }
}

void gBConditionsOneSecOneGroup::_shuffleGroups(int times)
{
    for(int i = 0; i < times; i ++)
        random_shuffle(this->conditionGroups.begin(), this->conditionGroups.end());

    this->_updateSectionBounds();
}

BOOL gBConditionsOneSecOneGroup::generateConditions()
{
    BOOL ret = groupBasedConditions::generateConditions();
    return ret;
}

BOOL gBConditionsOneSecOneGroup::generateAllConditions()
{
    BOOL ret = groupBasedConditions::generateAllConditions();
    return ret;
}

void gBConditionsOneSecOneGroup::_updateSectionBounds()
{
    vector<int>().swap(this->_sectionBounds);
    int weight = -1;

    for(int i = 0; i < (int) this->conditionGroups.size(); i ++)
    {
        int conditionIndex = (*this->conditionGroups[i])[0];
        int groupID = this->conditions[conditionIndex]->constraintGroupID - 1;
        weight += this->_groupRepeatTimes[groupID];

        this->_sectionBounds.push_back(weight); 
    }
}
