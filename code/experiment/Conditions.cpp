#include "stdafx.h"
#include "Conditions.h"
#include "TestObject.h"
#include <fstream>
#include <hash_map>

using namespace stdext;

Conditions::Conditions(string& filename, int numConditions,
        vector<TestObject *>templateObjects)
{
    this->numConditions = numConditions;

    // TODO: Support for the texture and motion later

    // Suppose the condition file is organized like this for one constraint
    // Object Names
    // Constraint weight
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

    // Build the mapping between the templateObjects and their names
    hash_map<string, TestObject *> nameMap;

    for(unsigned int i = 0; i < templateObjects.size(); i ++)
        nameMap[templateObjects[i]->getObjName()] = templateObjects[i];

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
            if(nameMap.find(name) != nameMap.end())
            {
                pNewConstraint->templateObjects.push_back(nameMap[name]);
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

        // TODO: Read Slant Range

        // TODO: Read Tilt Range
        
        // TODO: Read Height Range

        // TODO: Read Initial Z Aspect Ratio Range

        // TODO: Read Rotation Speed Range

        // TODO: Max Rotation Degree Range

        // TODO: Read Object Specific Parameters

        // Add the constaint
        this->addConstraint(*pNewConstraint);
    }
    fin.close();
}

Conditions::~Conditions(void)
{
    vector<condCons_t>& rConstraints = this->constraints;
    
    for(unsigned int i = 0; i < rConstraints.size(); i ++)
    {
        vector<TestObject *>& rTestObjects = rConstraints[i].templateObjects;
        while(!rTestObjects.empty())
        {
            TestObject *pObj = rTestObjects.back();
            
            delete pObj;
            rTestObjects.pop_back();
        }
    }
}

template<typename T>
BOOL Conditions::readRange(ifstream& fin, vector<T>& vec)
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
    }
    catch(ifstream::failure e)
    {
        MessageBox(NULL, e.what().c_str(), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void Conditions::addConstraint(condCons_t& constraint)
{
    this->constraints.push_back(constraint);
}

// Generate a condition according to the specified constraint
void Conditions::addCondition(int constraintIndex)
{

}

// Add the existing condition to the condition list
void Conditions::addCondition(cond_t& condition)
{
    this->conditions.push_back(condition);
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
        currAccWeight = prevAccWeight + this->constraints[i].weight;
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
