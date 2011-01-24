#include "StdAfx.h"
#include "groupBasedConditions.h"

groupBasedConditions::groupBasedConditions(ifstream& rFin, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    Conditions(rFin, numConditions, rObjectFactories, rScr)
{
}

groupBasedConditions::groupBasedConditions(ifstream& rFin,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    Conditions(rFin, rObjectFactories, rScr)
{
}

groupBasedConditions::~groupBasedConditions(void)
{
    vector<int> *pGroups;
    for(int i = 0; i < this->numGroups; i ++)
    {
        pGroups = this->constraintGroups.back();
        this->constraintGroups.pop_back();
        delete pGroups;

        pGroups = this->conditionGroups.back();
        this->conditionGroups.pop_back();
        delete pGroups;
    }
}

BOOL groupBasedConditions::initConditions()
{
    BOOL ret;
    ifstream& fin = this->overallFin;
    
    // Read condition repeat times per section
    fin >> this->conditionRepeatTimesPerSec;

    // Read textures
    ret = readTextures(fin);
    if(ret == FALSE)
        return FALSE;

    fin >> this->numGroups;

    for(int i = 0; i < this->numGroups; i ++)
    {
        vector<int> * pNewConstraintGroup = new vector<int>;

        pNewConstraintGroup->clear();
        this->constraintGroups.push_back(pNewConstraintGroup);
    }

    for(int i = 0; i < this->numGroups; i ++)
    {
        vector<int> * pNewConditionGroup = new vector<int>;

        pNewConditionGroup->clear();
        this->conditionGroups.push_back(pNewConditionGroup);
    }

    for(int i = 0; i < this->numGroups; i ++)
    {
        string constraintFilename;
        fin >> constraintFilename;

        try
        {
            ifstream constraintFin(constraintFilename.c_str());

            if(!constraintFin.good())
            {
                ostringstream ossError;
                ossError << "Error happens when reading constraint file " << constraintFilename
                    << "." << endl;
                MessageBox(NULL, (LPCSTR)(ossError.str().c_str()), NULL, MB_OK|MB_ICONERROR);
                return FALSE;
            }

            // Read constraints from file
            vector<condCons_t *> tempConstraints;
            tempConstraints.clear();

            ret = readConstraints(constraintFin, tempConstraints);
            if(ret == FALSE)
                return FALSE;

            for(unsigned int j = 0; j < tempConstraints.size(); j ++)
            {
                int index;
                tempConstraints[j]->groupID = i + 1;
                index = this->addConstraint(tempConstraints[j]);
                this->constraintGroups[i]->push_back(index);
            }
            constraintFin.close();
        }
        catch(ifstream::failure e)
        {
            ostringstream ossError;
            ossError << "Error happens when reading constraint file " << constraintFilename
                << "." << endl << "Error message is " << e.what();
            MessageBox(NULL, (LPCSTR)(ossError.str().c_str()), NULL, MB_OK|MB_ICONERROR);
            return FALSE;
        }
    }

    // Generate conditions according to constraints
    ret = this->generateConditions();
    if(ret == FALSE)
        return FALSE;

    return TRUE;
}
 
BOOL groupBasedConditions::generateConditions()
{
    int totalWeights = 0;

    this->clearConditions();

    for(unsigned int i = 0; i < this->constraintGroups.size(); i ++)
    {
        for(unsigned int j = 0; j < this->constraintGroups[i]->size(); j ++)
        {
            int constraintIndex = (*this->constraintGroups[i])[j];
            condCons_t& rConstraint = *this->constraints[constraintIndex];

            int quantity = rConstraint.weight * this->conditionRepeatTimesPerSec;

            for(int k = 0; k < quantity; k ++)
            {
                int index;
                index = this->addCondition(constraintIndex);
                this->conditionGroups[i]->push_back(index);
            }

            totalWeights += quantity;
        }
    }

    this->numConditions = totalWeights;

    this->shuffleConditions(7);

    return TRUE;
}

BOOL groupBasedConditions::clearConditions()
{
    BOOL ret;
    ret = Conditions::clearConditions();

    this->shuffledConditions.clear();

    for(int i = 0; i < this->numGroups; i ++)
        this->conditionGroups[i]->clear();

    return ret;
}

void groupBasedConditions::shuffleConditions(int times)
{

    for(int j = 0; j < times; j ++)
    {
        random_shuffle(this->conditionGroups.begin(), this->conditionGroups.end());

        for(unsigned int i = 0; i < this->conditionGroups.size(); i ++)
            random_shuffle(this->conditionGroups[i]->begin(), this->conditionGroups[i]->end());
    }
    this->updateShuffledConditions();
}

void groupBasedConditions::updateShuffledConditions()
{
    this->shuffledConditions.clear();

    for(unsigned int i = 0; i < this->conditionGroups.size(); i ++)
        for(unsigned int j = 0; j < this->conditionGroups[i]->size(); j ++)
        {
            shuffledConditions.push_back(this->conditions[(*this->conditionGroups[i])[j]]);        
        }
}

cond_t& groupBasedConditions::operator[](int &rhs)
{
    return *this->shuffledConditions[rhs];
}

const vector<cond_t *>& groupBasedConditions::getAllConditions()
{
    return this->shuffledConditions;
}
