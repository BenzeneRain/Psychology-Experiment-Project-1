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
    this->hInst = hInstance;
    this->currSecNo = 0;
    this->currTrialID = 0;
};

Experiment::~Experiment(void)
{

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
    this->initSystem();


    // Initialize the glut
    for(vector<Screen *>::iterator it = this->screens.begin();
            it != this->screens.end(); it ++)
    {
        Screen *pScreen = (Screen *) *it;
        if(!Experiment::debug)
        {
            pScreen->initGlut(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH |
                GLUT_MULTISAMPLE | GLUT_STENCIL,
                "Experiment", this->hBitmaps);
        }
        else
        {
            pScreen->initGlut(GLUT_RGB | GLUT_SINGLE,
                "Experiment", this->hBitmaps);
        }
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

    // dispose

    this->disposeSystem();
    
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
        MessageBox(NULL, "Fail to open the output file. The program will exit silently.", "Error", 0);
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
    // get the configurations
    ConfWnd *pConfWnd = ConfWnd::getInstance();

    this->subjectID = pConfWnd->subjectID;
    this->maxSecNo = pConfWnd->maxSecNo;
    this->experiMode = pConfWnd->experiMode;
    this->outFilename = pConfWnd->outFilename;
    this->trialsInOneSec = pConfWnd->trialsInOneSec;
    this->devMode = pConfWnd->devMode;
    this->strDate = pConfWnd->strDate;
    this->strTime = pConfWnd->strTime;

    // Register Test Objects Factories
    this->objectFactories.push_back(new CylinderFactory());

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize screen class
    static auto_ptr<Screen> apScreen;
    apScreen.reset(new Screen(this->devMode));

    Screen * pScreen = apScreen.get();
    this->screens.push_back(pScreen);

    // FIX: Load texture files
    HBITMAP hBitmap;
    hBitmap = (HBITMAP)::LoadImage(NULL, "./textures/voron_higher_resrgb.bmp", 
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(hBitmap == NULL)
    {
        MessageBox(NULL, "Fail to load the texture", NULL, MB_OK | MB_ICONERROR);
        return FALSE;
    }
    this->hBitmaps.push_back(hBitmap);


    // Initialize the conditions
    // FIX: the filename should not be here
    this->experimentConditions = new Conditions(string("config.txt"), this->maxSecNo * this->trialsInOneSec,
            this->objectFactories);
    this->experimentConditions->generateConditions();
    return TRUE;
}

BOOL Experiment::proceedExperiment()
{
    Trial *pTrial;

    // TODO: We can add Pre-Experiment Scene here
    // if needed

    //proceed trials
    while(this->currSecNo < this->maxSecNo)
    {
        int conditionID = this->currSecNo * this->trialsInOneSec
                + this->currTrialID;

        pTrial = new Trial(this->currTrialID,
                (*this->experimentConditions)[conditionID]);

        pTrial->startTrial();
        this->currTrialID ++;

        if(this->currTrialID >= this->trialsInOneSec)
        {
            this->currTrialID = 0;
            this->currSecNo ++;
        }

        delete pTrial;
    }

    //show post-experiment scene
    Scene *pScene;

    pScene = new PostExperimentScene();
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
    ossConf << "Number of trials in one section: " << this->trialsInOneSec << endl;

    ossConf << "Screen resolution: " << this->devMode.dmPelsWidth
            << " X " << this->devMode.dmPelsHeight << endl;

    // FIX: this is actually the max refresh rate, but we also need to record fps 
    ossConf << "Screen refresh rate: " << this->devMode.dmDisplayFrequency << "Hz" << endl;
    ossConf << "Bit per pixel of the Screen: " << this->devMode.dmBitsPerPel << endl;

    ossConf << "Experiment start time: " << this->strDate << " " << this->strTime << endl;

    ossConf << "Test object information: " << endl;
    
    // Output Test Object List
    //for(vector<TestObject *>::iterator it = this->stubObjects.begin();
    //    it != this->stubObjects.end(); it ++)
    //{
    //    string strTestObjDesc = ((TestObject *)(*it))->genObjDesc();
    //    ossConf << strTestObjDesc << endl;
    //}   


    // TODO: Output Condition list

    ret = this->writeOutputs(ossConf.str());

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

    // Delete Bitmap resources
    while(!this->hBitmaps.empty())
    {
        HBITMAP hBitmap = this->hBitmaps.back();
        DeleteObject(hBitmap);

        this->hBitmaps.pop_back();
    }

    // Delete Conditions
    delete this->experimentConditions;

    return TRUE;
}

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
