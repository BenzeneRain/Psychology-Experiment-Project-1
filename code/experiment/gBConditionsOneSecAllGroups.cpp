#include "StdAfx.h"
#include "gBConditionsOneSecAllGroups.h"

gBConditionsOneSecAllGroups::gBConditionsOneSecAllGroups(ifstream& rFin, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    groupBasedConditions(rFin, numConditions, rObjectFactories, rScr)
{
}

gBConditionsOneSecAllGroups::gBConditionsOneSecAllGroups(ifstream& rFin,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    groupBasedConditions(rFin, numConditions, rObjectFactories, rScr)
{

}

gBConditionsOneSecAllGroups::~gBConditionsOneSecAllGroups(void)
{

}

void gBConditionsOneSecAllGroups::shuffleConditions(int times)
{

    groupBasedConditions::shuffleConditions(times);
    this->updateShuffledConditions();
}

void gBConditionsOneSecAllGroups::updateShuffledConditions()
{
    this->shuffledConditions.clear();

    for(unsigned int i = 0; i < this->conditionGroups.size(); i ++)
        for(unsigned int j = 0; j < this->conditionGroups[i]->size(); j ++)
        {
            shuffledConditions.push_back(this->conditions[(*this->conditionGroups[i])[j]]);        
        }
}

cond_t& gBConditionsOneSecAllGroups::operator[](int &rhs)
{
    return *this->shuffledConditions[rhs];
}

const vector<cond_t *>& gBConditionsOneSecAllGroups::getAllConditions()
{
    return this->shuffledConditions;
}

BOOL gBConditionsOneSecAllGroups::clearConditions()
{
    BOOL ret;
    ret = groupBasedConditions::clearConditions();

    this->shuffledConditions.clear();
    vector<cond_t *>(this->shuffledConditions).swap(this->shuffledConditions);
    
    return ret;
}