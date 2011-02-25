// experiment.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "experiment.h"
#include "ConfWnd.h"
#include "Screen.h"
#include "Trial.h"
#include "TestObjectFactory.h"
#include "CylinderFactory.h"
#include "PostExperimentScene.h"
#include "gBConditionsOneSecAllGroups.h"
#include "gBConditionsOneSecOneGroup.h"

#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <io.h>
#include <direct.h>
using namespace std;

// This option is for debugging. Set it to 1 in order to enter debug mode
int const Experiment::debug = 0;

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nShowCmd)
{
    Experiment *pExperi = Experiment::getInstance(NULL);

    pExperi->startProgram();

    return 0;
}

Experiment::Experiment(HINSTANCE hInstance):
    xyz3D(3), xyz2D(3)
{
    Experiment::hInst = hInstance;
    this->currSecNo = 0;
    this->currTrialID = 0;
    this->currBlockID = 0;
    this->pScreen = NULL;
    this->experimentConditions = NULL;
};

Experiment::~Experiment(void)
{
    // dispose
    this->disposeSystem();
}

BOOL Experiment::startProgram()
{
    BOOL ret;

    // Open the Configuration Window for settings
    ConfWnd *pConfWnd = ConfWnd::getInstance();
    ret = pConfWnd->displayConfWnd(this->hInst);

    // If Configuration failed or Exit button is pressed, 
    // directly exit the program
    if(ret == FALSE)
    {
        return FALSE; 
    }

    // Initialize the system
    ret = this->initSystem();
    if(ret == FALSE)
    {
        // dispose
        this->disposeSystem();
        return FALSE; 
    }

    // Initialize the output file if in experiment mode
    if(this->experiMode == 0)
    {
        ret = this->initOutputFile();
        if(ret == FALSE)
            return FALSE;

        // Write fixed configurations to the output file
        this->recordConfigurations();
    }


    // main body
    this->proceedExperiment();

    return TRUE;
}

BOOL Experiment::initOutputFile()
{
    try
    {
        // Create the output directory if it is not existed
        int ret;
        ret = _access_s("./results", 0);
        if(ret != 0)
        {
            _mkdir("./results");
        }

        // open output file
        this->hFileOut.open(this->outFilename.c_str(), fstream::out);
        return TRUE;
    }
    catch(fstream::failure err)
    {
        MessageBox(NULL, (LPCSTR)"Fail to open the output file. The program will exit silently.", 
            (LPCSTR)"Error", 0);
        return FALSE;
    }
}

BOOL Experiment::closeOutputFile()
{
    if(this->hFileOut.is_open())
    {
        this->hFileOut.flush();
        this->hFileOut.close();
    }

    return TRUE;
}

BOOL Experiment::initSystem()
{
    BOOL ret = TRUE;
    // get the configurations
    ConfWnd *pConfWnd = ConfWnd::getInstance();

    this->subjectID = pConfWnd->subjectID;
    this->experiMode = pConfWnd->experiMode;
    this->outFilename = pConfWnd->outFilename;
    memcpy(&this->devMode, &pConfWnd->devMode, sizeof(DEVMODE));
    this->strDate = pConfWnd->strDate;
    this->strTime = pConfWnd->strTime;

    // Register Test Objects Factories
    this->objectFactories.push_back(new CylinderFactory());

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize screen class
    this->pScreen = new Screen(this->devMode);

    // Initialize Scene class
    Scene::reset();

    // Initialize the glut
    this->pScreen->initGlut(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH |
            GLUT_MULTISAMPLE | GLUT_STENCIL,
           "Experiment");

    // Initialize the conditions
    // FIX: the filename should not be hard coded here
    try
    {
        ifstream fin;
        if(this->experiMode == EXPERIMENT)
            fin.open("./config/config-experiment.txt");
        else
            fin.open("./config/config-practice.txt");

        string junk;
        junk.resize(256);

        if(!fin.good())
        {
            ostringstream ossError;
            ossError << "Error happens when reading constraint file config.txt." << endl;
            MessageBox(NULL, (LPCSTR)(ossError.str().c_str()), NULL, MB_OK|MB_ICONERROR);
            return FALSE;
        }

        // Read the conditionMode
        fin >> this->conditionMode;
        if(this->conditionMode > 2 || this->conditionMode < 0)
        {
            ostringstream ossError;
            ossError << "Wrong condition mode." << endl;
            ossError << "1: none group based conditions" << endl;
            ossError << "2: group based conditions, and all groups will appear in each section" << endl;
            ossError << "3: group based conditions, and only one group will appear in each section" << endl;
            MessageBox(NULL, (LPCSTR)(ossError.str().c_str()), NULL, MB_OK|MB_ICONERROR);
            return FALSE;
        }
        getline(fin, junk);

        // Read 3D and 2D object coordinates referring to the screen
        fin >> junk;
        fin >> this->xyz3D[0] >> this->xyz3D[1] >> this->xyz3D[2];
        fin >> junk;
        fin >> this->xyz2D[0] >> this->xyz2D[1] >> this->xyz2D[2];

        // Read minimal duration for each trial
        fin >> junk;
        fin >> this->minDurationForEachTrial;
        
        this->blocksPerSec = -1;
        switch(this->conditionMode)
        {
            case 0:
                // Read number of trials in each section
                fin >> junk >> this->trialsPerSec;

                this->experimentConditions =
                    new Conditions(fin, this->objectFactories, *this->pScreen);
                break;
            case 1:
                // Read number of sections
                fin >> junk >> this->maxSecNo; 

                this->experimentConditions =
                    new gBConditionsOneSecAllGroups(fin, this->objectFactories, *this->pScreen);
                break;
            case 2:
                // Read number of blocks for one section
                fin >> junk >> this->blocksPerSec;

                this->experimentConditions =
                    new gBConditionsOneSecOneGroup(fin, this->objectFactories, *this->pScreen);
                break;
            default:
                return FALSE;
        }

        ret = this->experimentConditions->initConditions();
        this->experimentConditions->shuffleConditions(1023);
        fin.close();
        if(ret == FALSE)
        {
            return FALSE;
        }
    }
    catch(ifstream::failure e)
    {
        MessageBox(NULL, (LPCSTR)(e.what()), NULL, MB_OK|MB_ICONERROR);
        return FALSE;
    }

    // Get the final condition list for one section
    const vector<cond_t *>& rConds = this->experimentConditions->getAllConditions(); 

    switch(this->conditionMode)
    {
        case 0:
            this->maxSecNo = (UINT)ceil((double)rConds.size() / (double)this->trialsPerSec);
            break;
        case 1:
            this->trialsPerSec = rConds.size();
            break;
        case 2:
            this->maxSecNo = (dynamic_cast<gBConditionsOneSecOneGroup *>(this->experimentConditions))->getTotalSectionNo();
            break;
        default:
            return FALSE;
    }
    return ret;
}

BOOL Experiment::proceedExperiment()
{
    Trial *pTrial;
    BOOL ret;

    // TODO: We can add Pre-Experiment Scene here
    // if needed

    //proceed trials
    this->experimentConditions->shuffleConditions(997);
    if(this->conditionMode == 2)
    {
        const vector<cond_t *>& rConds = this->experimentConditions->getAllConditions(); 
        this->trialsPerSec = rConds.size();

        if(this->blocksPerSec > 0)
            this->trialsPerBlock = (unsigned int)ceil((double)this->trialsPerSec / (double)this->blocksPerSec);
    }

    do
    {
        cond_t &rCond = (*this->experimentConditions)[(int&)this->currTrialID];
        rCond.reset();
        rCond.xyz3D = this->xyz3D;
        rCond.xyz2D = this->xyz2D;

        pTrial = new Trial(this->currTrialID, rCond, this->minDurationForEachTrial);

        ret = pTrial->startTrial();

        this->currTrialID ++;

        if(this->conditionMode == 2 && (this->blocksPerSec > 0) &&
            (this->currTrialID != 0 && (this->currTrialID % this->trialsPerBlock) == 0))
        {
            this->currBlockID ++;
        }

        if(this->currTrialID >= this->trialsPerSec)
        {
            this->currTrialID = 0;
            if(this->conditionMode == 2)
                this->currBlockID = 0;
            this->currSecNo ++;

            if(this->currSecNo < this->maxSecNo)
            {
                switch(this->conditionMode)
                {
                    case 1:
                        this->experimentConditions->shuffleConditions(997);
                        break;
                    case 2:
                        {
                            (dynamic_cast<gBConditionsOneSecOneGroup *>(this->experimentConditions))->stepSection();
                            this->experimentConditions->shuffleConditions(997);

                            const vector<cond_t *>& rConds = this->experimentConditions->getAllConditions(); 
                            this->trialsPerSec = rConds.size();

                            if(this->blocksPerSec > 0)
                                this->trialsPerBlock = (unsigned int)ceil((double)this->trialsPerSec / (double)this->blocksPerSec);
                            break;
                        }
                    default:
                        break;
                }
            }
        }

        delete pTrial;
    }while((this->currSecNo < this->maxSecNo) && (ret == TRUE));

    //show post-experiment scene
    Scene *pScene = new PostExperimentScene();
    pScene->startScene();
    delete pScene;

    return TRUE;
}

BOOL Experiment::isNewSection()
{
   if(this->currTrialID == 0)
   {
       return TRUE;
   }
   else
   {
       return FALSE;
   }

}

BOOL Experiment::isNewBlock()
{
    if(this->conditionMode != 2)
        return FALSE;

    if(this->blocksPerSec <= 0)
        return FALSE;

    if((this->currTrialID  % this->trialsPerBlock) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


BOOL Experiment::recordConfigurations()
{
    BOOL ret;
    
    ostringstream ossConf;

    ossConf << "Subject ID: " << this->subjectID << endl;
    ossConf << "Max section number: " << this->maxSecNo << endl;
    ossConf << "Number of trials in one section: " << this->trialsPerSec << endl;

    ossConf << "Screen resolution: " << this->devMode.dmPelsWidth
            << " X " << this->devMode.dmPelsHeight << endl;

    // FIX: this is actually the max refresh rate, but we also need to record fps 
    ossConf << "Screen refresh rate: " << this->devMode.dmDisplayFrequency << "Hz" << endl;
    ossConf << "Bit per pixel of the Screen: " << this->devMode.dmBitsPerPel << endl;

    ossConf << "Experiment start time: " << this->strDate << " " << this->strTime << endl;

    ossConf << endl;

    ret = this->writeOutputs(ossConf.str());
    if(ret == FALSE)
        return FALSE;
    
    ret = this->recordConstraints();

    return ret;
}

BOOL Experiment::recordConstraints()
{
    BOOL ret = TRUE;
    ostringstream ossCond;

    // Output Condition list
    const vector<condCons_t *>& rConstraints = this->experimentConditions->getAllConstraints(); 
    
    //FIX: Object maybe different, so they may have different description title
    if(rConstraints.size() != 0)
        ossCond << rConstraints[0]->genDescTitle();

    for(unsigned int i = 0; i < rConstraints.size(); i ++)
    {
        ossCond << rConstraints[i]->genDesc(); // Get object descriptions of the condition 
        ossCond << endl;
    }

    ossCond << endl;
    string constraintString = ossCond.str();
    ret = this->writeOutputs(constraintString);
    return ret;
}

BOOL Experiment::writeOutputs(string& strOutputs)
{
    this->hFileOut << strOutputs;
    this->hFileOut.flush();
    return TRUE;
}

BOOL Experiment::disposeSystem()
{
    // If in experiment mode, close the output file
    if(this->experiMode == 0)
    {
        this->closeOutputFile();
    }

    // Delete Registered Test Objects
    while(!this->objectFactories.empty())
    {
        TestObjectFactory *pObjectFactory = this->objectFactories.back();
        delete pObjectFactory;

        this->objectFactories.pop_back();
    }

    // Delete Screen 
    if(!this->pScreen)
        delete this->pScreen;

    // Delete Conditions
    if(!this->experimentConditions)
        delete this->experimentConditions;

    return TRUE;
}

HINSTANCE Experiment::hInst = NULL;
// Below are singalton implementations
auto_ptr<Experiment> Experiment::m_pInstance;

Experiment *Experiment::getInstance(HINSTANCE hInstance)
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new Experiment(hInstance));
    }

    return m_pInstance.get();
}
