#include "stdafx.h"
#include "Conditions.h"
#include "TestObjectFactory.h"
#include "TestObject.h"
#include "Screen.h"
#include <fstream>


using namespace stdext;

Conditions::Conditions(ifstream& rFin, int numConditions,
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    objectFactories(rObjectFactories), rScreen(rScr), overallFin(rFin)
{
    this->numConditions = numConditions;
    this->conditionRepeatTimesPerSec = 0;

    // Build the mapping between the object Factories and their product names
    this->objectFactoryNameMap.clear();
    for(unsigned int i = 0; i < this->objectFactories.size(); i ++)
        this->objectFactoryNameMap[this->objectFactories[i]->getProductName()] 
            = this->objectFactories[i];
}

Conditions::Conditions(ifstream& rFin, 
        vector<TestObjectFactory *>& rObjectFactories, Screen& rScr):
    objectFactories(rObjectFactories), rScreen(rScr), overallFin(rFin)
{
    this->numConditions = 0;
    this->conditionRepeatTimesPerSec = 0;
    
    // Build the mapping between the object Factories and their product names
    this->objectFactoryNameMap.clear();
    for(unsigned int i = 0; i < this->objectFactories.size(); i ++)
        this->objectFactoryNameMap[this->objectFactories[i]->getProductName()] 
            = this->objectFactories[i];
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

        // delete texture lists for each constraint
        while(!textureGroups.empty())
        {
            vector<string> *textureList = textureGroups.back();
            textureGroups.pop_back();
            delete textureList;
        }

        delete pCondCons;
    }

    // Dispose conditions
    this->clearConditions();
    
    // Dispose raw texture information
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
    // Display Mode
    // Constraint Weight 
    // Pitch Range (Slant)
    // Yaw Range
    // Roll Range (tilt)
    // Height Range 
    // Initial Z Aspect Ratio Range 
    // Rotation Speed Range 
    // Max Rotation Degree Range 
    // Object Specific Parameters     
    //
    // Display Mode has two possible values, one is C, meaning CONTINUOUS DISPLAY
    // , which corresponds to motion situation; the other one is D, meaning 
    // DISCRETE DISPLAY, which corresponds to no motion situation;
    //
    // if DISCRETE DISPLAY mode is chose, we need to provide another two values,
    // one is the seconds lasts for displaying the object; and the second one
    // is the seconds lasts while the object disappears
    // e.g. 1:
    // C
    //
    // e.g. 2:
    // D 2 2
    //
    //
    // Object Names format: "# of Object Names" "Name1" "Name2" ...  e.g.:
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
    
    ifstream& fin = this->overallFin;
    
    // Read condition repeat times per section
    fin >> this->conditionRepeatTimesPerSec;

    // Read textures
    ret = readTextures(fin);
    if(ret == FALSE)
        return FALSE;

    // Read constraints from file
    vector<condCons_t *> tempConstraints;
    ret = readConstraints(fin, tempConstraints);
    if(ret == FALSE)
        return FALSE;

    for(unsigned int i = 0; i < tempConstraints.size(); i ++)
        this->addConstraint(tempConstraints[i]);

    // Generate conditions according to constraints
    ret = this->generateConditions();
    if(ret == FALSE)
        return FALSE;

    return TRUE;
}

BOOL Conditions::readTextures(ifstream& fin)
{
    try
    {
        // Read Texture information (only support bmp file and RGB color setting now)
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
                case 'T': // routines for handling BMP file as texture
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
                case 'C': // routines for handling RGB color as texture
                    {
                        fin >> pTexture->color[0] >> pTexture->color[1] >> pTexture->color[2];
                        pTexture->type = 'C';
                        break;
                    }
                default: // routines for other types of textures
                    // FIX: Maybe we should warn the wrong type of the textures here
                    break;
            }

            // Store the texture information and build the mapping between their name to
            // the information
            this->textures.push_back(pTexture);
            textureMap[textureName] = iTextures;

        }

        // Initialize the textures
        // Include build the texture list if it is bmp file
        // build the color information if it is RGB color
        // Since some of the codes are opengl dependent, I 
        // put them in the screen class
        this->rScreen.initTextures(this->textures);
    }
    // anything wrong while reading from the configuration file
    // will be directed to here
    catch(ifstream::failure e) 
    {
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

BOOL Conditions::readConstraints(ifstream& fin, vector<condCons_t *>& rConstraints)
{
    try
    {
        // Read constraints
        int numConstraints;
        fin >> numConstraints;

        for(int iConstraint = 0; iConstraint < numConstraints; iConstraint ++)
        {
            int quantity;
            condCons_t *pNewConstraint = new condCons_t; 

            pNewConstraint->id = iConstraint + 1;
            pNewConstraint->groupID = 0;

            char dispType;
            fin >> dispType;

            switch(dispType)
            {
                case 'C': // Continues display
                    pNewConstraint->dispMode = CONTINUOUS_DISPLAY;
                    pNewConstraint->secDisplay = 0;
                    pNewConstraint->secBlackScreen = 0;
                    break;
                case 'D': // Discrete display
                    pNewConstraint->dispMode = DISCRETE_DISPLAY;
                    
                    float secs;
                    
                    // read seconds for displaying object
                    fin >> secs;
                    if(secs <= 0)
                    {
                        ostringstream ossError;
                        ossError << "The value for displaying object should not be less than or equal to zero in constraint "
                            << iConstraint;
                        string errorMsg = ossError.str();
                        MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                        return FALSE;
                    }
                    pNewConstraint->secDisplay = secs;

                    // read seconds for displaying black screen;
                    fin >> secs;
                    if(secs < 0)
                    {
                        ostringstream ossError;
                        ossError << "The value for displaying black screen should not be less than zero in constraint "
                            << iConstraint;
                        string errorMsg = ossError.str();
                        MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                        return FALSE;
                    }
                    pNewConstraint->secBlackScreen = secs;

                    break;
                default: // Unsupported display mode
                    ostringstream ossError;
                    ossError << "Unsupported display mode '" << dispType << "' in constraint "
                        << iConstraint << endl;
                    ossError << "Supported display mode:" << endl;
                    ossError << "C  ---   Continues Display" << endl;
                    ossError << "D  ---   Discrete Display" << endl;
                    string errorMsg = ossError.str();
                    MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                    return FALSE;
                    break;
            }

            // Read Object Names
            fin >> quantity;
            for(int i = 0; i < quantity; i ++)
            {
                string name;

                fin >> name;
                // Check if the object is supported or not
                if(this->objectFactoryNameMap.find(name) != this->objectFactoryNameMap.end())
                {
                    pNewConstraint->objectNames.push_back(name);
                }
                else
                {
                    ostringstream ossError;
                    ossError << "There is no built in object named " << name <<
                        "in Constaint " << iConstraint + 1 << ". Continue or not?";
                    string message = ossError.str();
                    // Let the user choose continue if the object is not found
                    // FIX: maybe we should exit instead of allowing the user
                    // ignore the error
                    int userChoice = 
                        MessageBox(NULL, (LPCSTR)(message.c_str()), NULL, MB_YESNO | MB_ICONWARNING);
                    if(userChoice == IDNO)
                        return FALSE;
                }
            }

            // Read Textures groups (groups of texture list) for the constraint
            // Each condition follow the constraint will randomly choose one
            // texture list from the group as textures to be used
            int numTextureGroups;
            fin >> numTextureGroups;
            for(int i = 0; i < numTextureGroups; i ++)
            {
                int numTextureItems;

                fin >> numTextureItems;

                // Build the texture lists for each constraint
                vector<string> *textureList = new vector<string>(numTextureItems);

                // Read the texture list information
                for(int j = 0; j < numTextureItems; j ++)
                {
                    string textureName;
                    fin >> textureName;

                    // Check if the selected texture is existed or not
                    if(this->textureMap.find(textureName) != this->textureMap.end())
                    {
                        (*textureList)[j] = textureName;
                    }
                    else
                    {
                        ostringstream ossError;
                        ossError << "There is no texture named " << textureName << 
                            " in Constraint " << iConstraint + 1 << ". Continue or not?";
                        string message = ossError.str();
                        // Let the user choose continue if the texture is not found
                        // FIX: maybe we should exit instead of allowing the user
                        // ignore the error
                        int userChoice = 
                            MessageBox(NULL, (LPCSTR)(message.c_str()), NULL, MB_YESNO | MB_ICONWARNING);
                        if(userChoice == IDNO)
                            return FALSE;
                    }
                }

                pNewConstraint->textureGroups.push_back(textureList);
            }

            // Read Constraint Weight
            // FIX: Need a check if the input weight value is valid or not
            int weight;
            fin >> weight;
            BOOL ret;
            
            pNewConstraint->weight = weight;

            // Read Pitch Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->pitchRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("pitch", iConstraint + 1);
                return FALSE;
            }

            // Read Yaw Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->yawRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("yaw", iConstraint + 1);
                return FALSE;
            }

            // Read Roll Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->rollRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("roll", iConstraint + 1);
                return FALSE;
            }

            // Read Height Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->heightRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("height", iConstraint + 1);
                return FALSE;
            }

            // Read Initial Z Aspect Ratio Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->initZAsptRatioRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("initial aspect ratio on z axis", iConstraint + 1);
                return FALSE;
            }

            // Read Rotation Speed Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->rotSpeedRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("rotation speed", iConstraint + 1);
                return FALSE;
            }

            // Max Rotation Degree Range
            ret = this->readRange<GLfloat>(fin, pNewConstraint->maxRotDegRange);
            if(ret == FALSE)
            {
                this->printReadRangeError("max rotation degree", iConstraint + 1);
                return FALSE;
            }

            // Check the max rotation speed should be less than the smallest max rotation degree
            // in no motion mode
            GLfloat maxRotSpeed = *max_element(pNewConstraint->rotSpeedRange.range.begin(),
                pNewConstraint->rotSpeedRange.range.end());
            GLfloat minRotDegree = *min_element(pNewConstraint->maxRotDegRange.range.begin(),
                pNewConstraint->maxRotDegRange.range.end());

            if((maxRotSpeed > (minRotDegree * 2)) && pNewConstraint->dispMode == DISCRETE_DISPLAY)
            {
                ostringstream ossError;
                ossError << "The max possible speed is larger than the smallest possible max rotation degree in constraint " 
                    << iConstraint << "in no motion mode";
                string errorMsg = ossError.str();
                MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                return FALSE;
            }

            if(minRotDegree < 0.5f)
            {
                ostringstream ossError;
                ossError << "The smallest possible max rotation degree in constraint " 
                    << iConstraint << "should not be smaller than 0.5";
                string errorMsg = ossError.str();
                MessageBox(NULL, (LPSTR)(errorMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
                return FALSE;
            }

            // Read Object Specific Parameters
            for(unsigned int i = 0; i < pNewConstraint->objectNames.size(); i ++)
            {
                if(!pNewConstraint->objectNames[i].compare("Cylinder"))
                {
                    ret = this->cylinderParameterReadingFunction(fin, *pNewConstraint, iConstraint + 1);
                    if(ret == FALSE)
                        return FALSE;
                }
            }

            // Add the constaint
            rConstraints.push_back(pNewConstraint);
        }
    }
    // anything wrong while reading from the configuration file
    // will be directed to here
    catch(ifstream::failure e) 
    {
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

// Read information that is in range
template<typename T>
BOOL Conditions::readRange(ifstream& fin, rangeType<T>& vec)
{
    char rangeType;

    try
    {
        // read the type of the range
        fin >> rangeType;
        switch(rangeType)
        {
            case 'R':
                T lowerBound, upperBound;

                fin >> lowerBound >> upperBound;
                if(lowerBound > upperBound)
                    return FALSE;

                vec.range.push_back(lowerBound);
                vec.range.push_back(upperBound);
                vec.type = 'R';
                break;
            case 'S':
                // if the range is defined by a set of possible values,
                // it will be directed here
                {
                    int size;
                    fin >> size;

                    for(int i = 0; i < size; i ++)
                    {
                        T value;
                        fin >> value;
                        vec.range.push_back(value);
                    }

                    vec.type = 'S';
                    break;
                }
            default:
                return FALSE;
        }

    }
    catch(ifstream::failure e) // FIX: just throw the error
    {
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

// it will be called if the range reading function is successfully 
// executed
void Conditions::printReadRangeError(string name, int constraintID)
{
    ostringstream ossError;
    ossError << "Invalid " << name << " range for constraint " << constraintID;
    string message = ossError.str();
    MessageBox(NULL, (LPCSTR)(message.c_str()), NULL, MB_OK | MB_ICONERROR);
}

// Add a constraint to the constraint vector
int Conditions::addConstraint(condCons_t* pConstraint)
{
    this->constraints.push_back(pConstraint);
    return this->constraints.size() - 1;
}

// Generate a condition according to the specified constraint
int Conditions::addCondition(int constraintIndex)
{
   int randIndex;
   cond_t *pNewCondition = new cond_t;

   // Randomly select a texture list from the texture groups of the contraint
   // as the textures for the new condition
   randIndex = rand() % this->constraints[constraintIndex]->textureGroups.size();
   vector<string> *textureList = this->constraints[constraintIndex]->textureGroups[randIndex];

   for(unsigned int i = 0; i < textureList->size(); i ++)
   { 
        // firstly, get the name of the texture from the texture list
        // secondly, get the index of the texture according to its name 
        int index = this->textureMap[(*textureList)[i]];

        texture_t *pTexture = new texture_t;
        pTexture->name = this->textures[index]->name;
        switch(this->textures[index]->type)
        {
            case 'T':
                // if the texture is bmp file, store the opengl texture id
                pTexture->type = 'T';
                pTexture->textureID = this->rScreen.texIDs[index];
                pNewCondition->textures.push_back(pTexture);
                break;
            case 'C':
                // if the texture is RGB color, store the RGB value
                pTexture->type = 'C';
                pTexture->color[0] = this->rScreen.colorIDs[index][0];
                pTexture->color[1] = this->rScreen.colorIDs[index][1];
                pTexture->color[2] = this->rScreen.colorIDs[index][2];
                pNewCondition->textures.push_back(pTexture);
                break;
            default:
                // do nothing for other cases
                break;
        }
   }

   // Produce object
   randIndex = rand() % this->constraints[constraintIndex]->objectNames.size();
   TestObjectFactory *pFactory = this->objectFactoryNameMap[
       this->constraints[constraintIndex]->objectNames[randIndex]];
   // FIX: Check if the pRealObject is NULL or not
   pNewCondition->pRealObject = pFactory->createObject(*this->constraints[constraintIndex], pNewCondition->textures);

   // Set the random parameters
   pNewCondition->pRealObject->setRandPara();

   pNewCondition->repeatTime = this->conditionRepeatTimesPerSec;
   
   pNewCondition->dispMode = this->constraints[constraintIndex]->dispMode;
   if(pNewCondition->dispMode == DISCRETE_DISPLAY)
   {
       pNewCondition->secDisplay = this->constraints[constraintIndex]->secDisplay;
       pNewCondition->secBlackScreen = this->constraints[constraintIndex]->secBlackScreen;
   }
   else
   {
       pNewCondition->secDisplay = 0.0f;
       pNewCondition->secBlackScreen = 0.0f;
   }

   pNewCondition->constraintID = this->constraints[constraintIndex]->id;
   pNewCondition->constraintGroupID = this->constraints[constraintIndex]->groupID;

   this->conditions.push_back(pNewCondition);

   return this->conditions.size() - 1;
}

// Add the existing condition to the condition list
int Conditions::addCondition(cond_t* pCondition)
{
   this->conditions.push_back(pCondition);
   return this->conditions.size() - 1;
}

// shuffle all the conditions
void Conditions::shuffleConditions(int times)
{
    for(int i = 0; i < times; i ++)
        random_shuffle(this->conditions.begin(), this->conditions.end());
}

// generate the conditions
BOOL Conditions::generateConditions()
{
    int totalWeights = 0;

    this->clearConditions();

    for(unsigned int i = 0; i < this->constraints.size(); i ++)
    {
        int quantity = this->constraints[i]->weight * this->conditionRepeatTimesPerSec;

        for(int j = 0; j < quantity; j ++)
            this->addCondition(i);

        totalWeights += this->constraints[i]->weight * this->conditionRepeatTimesPerSec;
    }

    this->numConditions = totalWeights;

    this->shuffleConditions(7);
    return TRUE;
}

BOOL Conditions::clearConditions()
{
    vector<cond_t *>& rConditions = this->conditions;

    while(!rConditions.empty())
    {
        cond_t *pCondition = rConditions.back();
        TestObject *pObject = pCondition->pRealObject;
        vector<texture_t *>& rVecTexture = pCondition->textures;
        rConditions.pop_back();

        // Delete texture information used in each condition
        while(!rVecTexture.empty())
        {
           texture_t *pTexture = rVecTexture.back();
           rVecTexture.pop_back();
           delete pTexture;
        }

        // Delete object used in each condition
        delete pObject;
        delete pCondition;
    }

    return TRUE;
}

const vector<condCons_t *>& Conditions::getAllConstraints()
{
    return this->constraints;
}

const vector<cond_t *>& Conditions::getAllConditions()
{
    return this->conditions;
}

BOOL Conditions::cylinderParameterReadingFunction(ifstream& fin, condCons_t& constraint, int constraintID)
{
    BOOL ret;
    
    // Read Radius Range
    ret = this->readRange<GLfloat>(fin, constraint.radiusRange);
    if(ret == FALSE)
    {
        this->printReadRangeError("radius(Cylinder)", constraintID);
        return FALSE;
    }

    return TRUE;
}

cond_t& Conditions::operator[](int &rhs)
{
    return *this->conditions[rhs];
}

void cond_t::reset()
{
    this->pRealObject->reset();
}
