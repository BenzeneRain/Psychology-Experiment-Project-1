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

#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>

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

Experiment::Experiment(HINSTANCE hInstance)
{
    Experiment::hInst = hInstance;
    this->currSecNo = 0;
    this->currTrialID = 0;
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
    //this->maxSecNo = pConfWnd->maxSecNo;
    this->experiMode = pConfWnd->experiMode;
    this->outFilename = pConfWnd->outFilename;
   // this->trialsPerSec = pConfWnd->trialsPerSec;
    this->devMode = pConfWnd->devMode;
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
    if(!Experiment::debug)
    {
        this->pScreen->initGlut(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH |
                GLUT_MULTISAMPLE | GLUT_STENCIL,
                "Experiment");
    }
    else
    {
        this->pScreen->initGlut(GLUT_RGB | GLUT_SINGLE,
                "Experiment");
    }

    // Initialize the conditions
    // FIX: the filename should not be hard coded here
    this->experimentConditions = new Conditions(string("config.txt"), 
            this->objectFactories, *this->pScreen);
    ret = this->experimentConditions->initConditions();

    // Generate the final condition list for one section
    const vector<cond_t *>& rConds = this->experimentConditions->getAllConditions(); 

    this->trialsPerSec = rConds.size();
    this->maxSecNo = this->experimentConditions->numSections; 

    return ret;
}

BOOL Experiment::proceedExperiment()
{
    Trial *pTrial;
    BOOL ret;

    // TODO: We can add Pre-Experiment Scene here
    // if needed

    //proceed trials
    ostringstream ossSecInfo;
    ossSecInfo << "Section " << this->currSecNo + 1 << endl;
    this->writeOutputs(ossSecInfo.str());
    ossSecInfo.clear();
    do
    {
        pTrial = new Trial(this->currTrialID,
                (*this->experimentConditions)[(int&)this->currTrialID]);

        ret = pTrial->startTrial();

        this->currTrialID ++;
        if(this->currTrialID >= this->trialsPerSec)
        {
            this->currTrialID = 0;
            this->currSecNo ++;

            if(this->currSecNo < this->maxSecNo)
            {
                ossSecInfo << "Section " << this->currSecNo + 1 << endl;
                this->writeOutputs(ossSecInfo.str());
                ossSecInfo.clear();
                this->experimentConditions->shuffleConditions();
                this->recordConditions();
                this->writeOutputs(string("\n"));
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
    return ret;
}

BOOL Experiment::recordConditions()
{
    BOOL ret = TRUE;
    ostringstream ossCond;

    // Output Condition list
    const vector<cond_t *>& rConds = this->experimentConditions->getAllConditions(); 
    for(unsigned int i = 0; i < rConds.size(); i ++)
    {
        ossCond << i + 1 << " "; // Condition ID

        ossCond << rConds[i]->pRealObject->genObjDesc(); // Get object descriptions of the condition 
        ossCond << endl;
    }

    ossCond << endl;
    ret = this->writeOutputs(ossCond.str());
    return ret;
}

BOOL Experiment::writeOutputs(string& strOutputs)
{
    this->hFileOut << strOutputs;

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
