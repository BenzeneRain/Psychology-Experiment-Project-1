#include "stdafx.h"
#include "Conditions.h"
#include "TestObjectFactory.h"
#include "TestObject.h"
#include <fstream>


using namespace stdext;

Conditions::Conditions(string& filename, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories):
    objectFactories(rObjectFactories)
{
    this->numConditions = numConditions;

    // TODO: Support for the texture and motion later

    // Suppose the condition file is organized like this for one constraint
    // Object Names
    // Constraint Weight
    // Slant Range
    // Tilt Range
    // Height Range
    // Initial Z Aspect Ratio Range
    // Rotation Speed Range
    // Max Rotation Degree Range
    // Object Specific Parameters     
    //
    // P.S:
    // Object Names format: "# of Object Names" "Name1" "Name2" ...
    // e.g.: 2 Cylinder Cube
    //
    // Constaint weight is a integer. Suppose constraint 1 has weight w1, ... constraint i has weight i ..., then in all the later
    // generated conditions, w1/(w1 + w2 + ...) percent of the conditions is under constraint 1, ... , wi/(w1 + w2 + ...) percent of
    // the conditions is under constraint i, ...
    //
    // Support for the Range format 1 later
    // //XXXX Range format 1 (range is defined by a lower bound and a upper bound): "R" "Lower Bound" "Upper Bound"
    // XXXX Range format 2 (range is defined by a set of possible values): "S" "number of values in the set" "value 1" "value 2" ...
    // e.g. 1: R 0.8 1.2
    // e.g. 2: S 3 0.9 1.0 1.1
    //
    // Object Specific Parameters are in the order of the objects appeared in the "Object Names". Each object can have
    // multiple parameters, and it can also have none parameters. e.g. Cylinder has a parameter for radius range
    
    // Assume the Configuration file is complete and valid

    // Build the mapping between the object Factories and their product names
    this->objectFactoryNameMap.clear();
    for(unsigned int i = 0; i < this->objectFactories.size(); i ++)
        this->objectFactoryNameMap[this->objectFactories[i]->getProductName()] 
            = this->objectFactories[i];

    ifstream fin(filename.c_str());
    while(fin.good())
    {
        int quantity;
        condCons_t *pNewConstraint = new condCons_t; 

        // Read Object Names
        fin >> quantity;
        for(int i = 0; i < quantity; i ++)
        {
            string name;
            
            fin >> name;
            if(this->objectFactoryNameMap.find(name) != this->objectFactoryNameMap.end())
            {
                pNewConstraint->objectNames.push_back(name);
            }
            else
            {
                string message("There is no built in object named ");
                message += name;
                MessageBox(NULL, message.c_str(), NULL, MB_OK | MB_ICONWARNING);
            }
        }

        // Read Constraint Weight
        int weight;
        fin >> weight;

        pNewConstraint->weight = weight;

        // Read Slant Range
        this->readRange<GLfloat>(fin, pNewConstraint->slantRange, pNewConstraint->slantRangeType);

        // Read Tilt Range
        this->readRange<GLfloat>(fin, pNewConstraint->tiltRange, pNewConstraint->tiltRangeType);
        
        // Read Height Range
        this->readRange<GLfloat>(fin, pNewConstraint->heightRange, pNewConstraint->heightRangeType);

        // Read Initial Z Aspect Ratio Range
        this->readRange<GLfloat>(fin, pNewConstraint->initZAsptRatioRange, pNewConstraint->initZAsptRatioRangeType);

        // Read Rotation Speed Range
        this->readRange<GLfloat>(fin, pNewConstraint->rotSpeedRange, pNewConstraint->rotSpeedRangeType);

        // Max Rotation Degree Range
        this->readRange<GLfloat>(fin, pNewConstraint->maxRotDegRange, pNewConstraint->maxRotDegRangeType);

        // Read Object Specific Parameters
        for(unsigned int i = 0; i < pNewConstraint->objectNames.size(); i ++)
        {
            if(!pNewConstraint->objectNames[i].compare("Cylinder"))
            {
                this->cylinderParameterReadingFunction(fin, *pNewConstraint);
            }
        }

        // Add the constaint
        this->addConstraint(pNewConstraint);
    }
    fin.close();
}

Conditions::~Conditions(void)
{
    // Dispose constraints
    vector<condCons_t *>& rConstraints = this->constraints;
    
    while(!rConstraints.empty())
    {
        condCons_t *pCondCons = rConstraints.back();
        rConstraints.pop_back();
        delete pCondCons;
    }

    // Dispose conditions
    vector<cond_t *>& rConditions = this->conditions;

    while(!rConditions.empty())
    {
        cond_t *pCondition = rConditions.back();
        TestObject *pObject = pCondition->pRealObject;
        rConditions.pop_back();

        delete pObject;
        delete pCondition;
    }
}

template<typename T>
BOOL Conditions::readRange(ifstream& fin, vector<T>& vec, char& type)
{
    char rangeType;

    try
    {
        fin >> rangeType;
        switch(rangeType)
        {
            case 'R':
                break;
            case 'S':
                {
                    int size;
                    fin >> size;

                    for(int i = 0; i < size; i ++)
                    {
                        T value;
                        fin >> value;
                        vec.push_back(value);
                    }
                    break;
                }
            default:
                return FALSE;
        }

        type = rangeType;
    }
    catch(ifstream::failure e)
    {
        MessageBox(NULL, e.what(), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void Conditions::addConstraint(condCons_t* pConstraint)
{
    this->constraints.push_back(pConstraint);
}

// Generate a condition according to the specified constraint
void Conditions::addCondition(int constraintIndex)
{
   int randIndex;
   randIndex = rand() % this->constraints[constraintIndex]->objectNames.size();

   TestObjectFactory *pFactory = this->objectFactoryNameMap[
       this->constraints[constraintIndex]->objectNames[randIndex]];

   cond_t *pNewCondition = new cond_t;

   pNewCondition->pRealObject = pFactory->createObject(*this->constraints[constraintIndex]);
   pNewCondition->pRealObject->setRandPara();

   this->conditions.push_back(pNewCondition);
}

// Add the existing condition to the condition list
void Conditions::addCondition(cond_t* pCondition)
{
    this->conditions.push_back(pCondition);
}

void Conditions::shuffleConditions()
{
    random_shuffle(this->conditions.begin(), this->conditions.end());
}

void Conditions::generateConditions()
{
    int totalWeights = 0;
    vector<int> accumulateWeight;
    int prevAccWeight = 0;
    int currAccWeight = 0;

    for(unsigned int i = 0; i < this->constraints.size(); i ++)
    {
        currAccWeight = prevAccWeight + this->constraints[i]->weight;
        accumulateWeight.push_back(currAccWeight - 1);
        prevAccWeight = currAccWeight;
    }

    totalWeights = currAccWeight;

    for(int i = 0; i < this->numConditions; i ++)
    {
        int randomWeight = rand() % totalWeights; 

        vector<int>::iterator it = 
            lower_bound(accumulateWeight.begin(), accumulateWeight.end(), randomWeight);

        int index = it - accumulateWeight.begin();
             
        this->addCondition(index);
    }

    this->shuffleConditions();
}

const vector<condCons_t *>& Conditions::getAllConstraints()
{
    return this->constraints;
}

const vector<cond_t *>& Conditions::getAllConditions()
{
    return this->conditions;
}

BOOL Conditions::cylinderParameterReadingFunction(ifstream& fin, condCons_t& constraint)
{
    BOOL ret;
    
    // Read Radius Range
    ret = this->readRange<GLfloat>(fin, constraint.radiusRange, constraint.radiusRangeType);

    return ret;
}

cond_t& Conditions::operator[](int &rhs)
{
    return *this->conditions[rhs];
}
