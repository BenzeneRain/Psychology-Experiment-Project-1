#include "stdafx.h"
#include "Conditions.h"
#include "TestObjectFactory.h"
#include "TestObject.h"
#include "Screen.h"
#include <fstream>


using namespace stdext;

Conditions::Conditions(string& rFilename, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    objectFactories(rObjectFactories), rScreen(rScr), filename(rFilename)
{
    this->numConditions = numConditions;
}

Conditions::~Conditions(void)
{
    // Dispose constraints
    vector<condCons_t *>& rConstraints = this->constraints;
    
    while(!rConstraints.empty())
    {
        condCons_t *pCondCons = rConstraints.back();
        rConstraints.pop_back();

        vector< vector<string> *>& textureGroups = pCondCons->textureGroups;

        while(!textureGroups.empty())
        {
            vector<string> *textureList = textureGroups.back();
            textureGroups.pop_back();
            delete textureList;
        }

        delete pCondCons;
    }

    // Dispose conditions
    vector<cond_t *>& rConditions = this->conditions;

    while(!rConditions.empty())
    {
        cond_t *pCondition = rConditions.back();
        TestObject *pObject = pCondition->pRealObject;
        vector<texture_t *>& rVecTexture = pCondition->textures;
        rConditions.pop_back();

        while(!rVecTexture.empty())
        {
           texture_t *pTexture = rVecTexture.back();
           rVecTexture.pop_back();
           delete pTexture;
        }

        delete pObject;
        delete pCondition;
    }
    
    // Dispose textures
    while(!this->textures.empty())
    {
        rTexture_t *pTexture = this->textures.back();

        if(pTexture->type == 'T')
        {
            HBITMAP hBitmap = pTexture->hBitmap;
            DeleteObject(hBitmap);
        }
        this->textures.pop_back();

        delete pTexture;
    }
}

BOOL Conditions::initConditions()
{
    BOOL ret;

    // Build the mapping between the object Factories and their product names
    this->objectFactoryNameMap.clear();
    for(unsigned int i = 0; i < this->objectFactories.size(); i ++)
        this->objectFactoryNameMap[this->objectFactories[i]->getProductName()] 
            = this->objectFactories[i];

    // Read constraints from file
    ifstream fin(this->filename.c_str());
    ret = readConstraints(fin);
    if(ret == FALSE)
        return FALSE;

    // Generate conditions according to constraints
    this->generateConditions();

    fin.close();
    return TRUE;
}

BOOL Conditions::readConstraints(ifstream& fin)
{
    // TODO: What about the objects without textures but pure color

    // First line has a number T, indicating the number of texutures used
    //
    // Following T lines, each line has three components:
    // the first one is the user-defined name(contain no space) for the texture; 
    // the second one is and the texture's type, 'T' means it is from a texture file,
    // and 'C' means it is a color;
    // if the texture type is 'T', the third one is the relative path to the program
    // plus the filename; if the texture type is 'C', the third one has three integers
    // from 0 to 255, which indicating R, G and B
    // e.g.
    // t1 T ./texture/stone.bmp 
    // c1 C 255 255 255
    // the user defined texture name will be used in constraint description
    // 
    // The T + 2 line has a number C indicating the number of constraints. Then
    // the description of C constraints follows. 
    //
    // The constraint description is organized like this:
    // Object Names
    // Number of texture groups TG, and TG lines of texture list follows:
    // Texture list(start with a number indicating the number of items in the list, and 
    // then use user-defined texture name which are separated by space. The order 
    // of names in the list will affect the final result. e.g. if you need all 
    // three faces of the cylinder use the same texture t1, then it should be
    // 3 t1 t1 t1
    // )
    //
    // Constraint Weight 
    // Slant Range 
    // Tilt Range 
    // Height Range 
    // Initial Z Aspect Ratio Range 
    // Rotation Speed Range 
    // Max Rotation Degree Range 
    // Object Specific Parameters     
    //
    // P.S: Object Names format: "# of Object Names" "Name1" "Name2" ...  e.g.:
    // 2 Cylinder Cube
    // 
    // When generating a condition using specified constraint, it will randomly
    // choose a texture list from the texture group.
    //
    // Constaint weight is a integer. Suppose constraint 1 has weight w1, ...
    // constraint i has weight i ..., then in all the later generated
    // conditions, w1/(w1 + w2 + ...) percent of the conditions is under
    // constraint 1, ... , wi/(w1 + w2 + ...) percent of the conditions is
    // under constraint i, ...
    //
    // Support for the Range format 1 later 
    // XXXX Range format 1 (range is defined by a lower bound and a upper bound):
    // "R" "Lower Bound" "Upper Bound"
    // e.g.: R 0.8 1.2
    //
    // XXXX Range format 2 (range is defined by a set of possible values):
    // "S" "number of values in the set" "value 1" "value 2" ... 
    // e.g.: S 3 0.9 1.0 1.1
    //
    // Object Specific Parameters are in the order of the objects appeared in
    // the "Object Names". Each object can have multiple parameters, and it can
    // also have none parameters. e.g. Cylinder has only one parameter for radius
    // range

    // Assume the Configuration file is complete and valid

    try
    {
        // Read Texture information (only support bmp file now)
        int numTextures;
        fin >> numTextures;

        this->textureMap.clear();
        for(int iTextures = 0; iTextures < numTextures; iTextures ++)
        {
            string textureName;
            string filename;
            char textureType;
            
            fin >> textureName >> textureType;
            rTexture_t *pTexture = new rTexture_t;  
            pTexture->name = textureName;

            switch(textureType)
            {
                case 'T':
                    {
                        fin >> filename;

                        HBITMAP hBitmap = NULL;
                        hBitmap = (HBITMAP)::LoadImage(NULL, (LPCSTR)(filename.c_str()), 
                                IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                        if(hBitmap == NULL)
                        {
                            string errorMsg = "Fail to load the texture ";
                            errorMsg += filename;
                            MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                            return FALSE;
                        }

                        pTexture->type = 'T';
                        pTexture->hBitmap = hBitmap;
                        break;
                    }
                case 'C':
                    {
                        fin >> pTexture->color[0] >> pTexture->color[1] >> pTexture->color[2];
                        pTexture->type = 'C';
                        break;
                    }
                default:
                    break;
            }

            this->textures.push_back(pTexture);
            textureMap[textureName] = iTextures;

        }

        this->rScreen.initTextures(this->textures);
    
        // Read constraints
        int numConstraints;
        fin >> numConstraints;

        for(int iConstraint = 0; iConstraint < numConstraints; iConstraint ++)
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
                    MessageBox(NULL, (LPCSTR)(message.c_str()), NULL, MB_OK | MB_ICONWARNING);
                }
            }

            // Read Textures for the constraint
            int numTextureGroups;
            fin >> numTextureGroups;
            for(int i = 0; i < numTextureGroups; i ++)
            {
                int numTextureItems;

                fin >> numTextureItems;
                vector<string> *textureList = new vector<string>(numTextureItems);

                for(int j = 0; j < numTextureItems; j ++)
                {
                    string textureName;
                    fin >> textureName;

                    if(this->textureMap.find(textureName) != this->textureMap.end())
                    {
                        (*textureList)[j] = textureName;
                    }
                    else
                    {
                        string message("There is no texture named ");
                        message += textureName;
                        MessageBox(NULL, (LPCSTR)(message.c_str()), NULL, MB_OK | MB_ICONWARNING);
                    }
                }

                pNewConstraint->textureGroups.push_back(textureList);
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
    }
    catch(ifstream::failure e)
    {
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
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
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
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
   cond_t *pNewCondition = new cond_t;

   // Generate texture mappings
   randIndex = rand() % this->constraints[constraintIndex]->textureGroups.size();
   vector<string> *textureList = this->constraints[constraintIndex]->textureGroups[randIndex];

   for(unsigned int i = 0; i < textureList->size(); i ++)
   { 
        int index = this->textureMap[(*textureList)[i]];

        texture_t *pTexture = new texture_t;
        pTexture->name = this->textures[index]->name;
        switch(this->textures[index]->type)
        {
            case 'T':
                pTexture->type = 'T';
                pTexture->textureID = this->rScreen.texIDs[index];
                pNewCondition->textures.push_back(pTexture);
                break;
            case 'C':
                pTexture->type = 'C';
                pTexture->color[0] = this->rScreen.colorIDs[index][0];
                pTexture->color[1] = this->rScreen.colorIDs[index][1];
                pTexture->color[2] = this->rScreen.colorIDs[index][2];
                pNewCondition->textures.push_back(pTexture);
                break;
            default:
                break;
        }
   }

   // Produce object
   randIndex = rand() % this->constraints[constraintIndex]->objectNames.size();
   TestObjectFactory *pFactory = this->objectFactoryNameMap[
       this->constraints[constraintIndex]->objectNames[randIndex]];

   // FIX: Check if the pRealObject is NULL or not
   pNewCondition->pRealObject = pFactory->createObject(*this->constraints[constraintIndex], pNewCondition->textures);
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
