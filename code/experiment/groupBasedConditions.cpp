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
    string junk;
    
    // Read condition repeat times per section
    fin >> junk >> this->conditionRepeatTimesPerSec;

    // Read textures
    ret = readTextures(fin);
    if(ret == FALSE)
        return FALSE;

    fin >> junk >> this->numGroups;

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
        fin >> junk >> constraintFilename;

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

    return ret;
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

    //this->shuffleConditions(7);

    return TRUE;
}

BOOL groupBasedConditions::generateAllConditions()
{
    int prevIndex=-1;
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
                index = this->addAllConditionsFromConstraint(constraintIndex);

                for(int p = prevIndex + 1; p <= index; p ++)
                    this->conditionGroups[i]->push_back(p);

                prevIndex = index;
            }
        }
    }

    this->numConditions = this->conditions.size();

    //this->shuffleConditions(1023);

    return TRUE;
}

BOOL groupBasedConditions::clearConditions()
{
    BOOL ret;
    ret = Conditions::clearConditions();

    for(int i = 0; i < this->numGroups; i ++)
    {
        this->conditionGroups[i]->clear();
        vector<int>(*this->conditionGroups[i]).swap(*this->conditionGroups[i]);
    }

    return ret;
}
