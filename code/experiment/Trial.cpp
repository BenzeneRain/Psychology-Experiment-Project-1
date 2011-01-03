#include "stdafx.h"
#include "Trial.h"
#include "Scene.h"
#include "PreTrialScene.h"

Trial::Trial()
{
    this->trialID = 0;
    this->currState = IDLE;
}

Trial::~Trial(void)
{
}

BOOL Trial::startTrial()
{
    this->proceedNextScene();
    return TRUE;
}

BOOL Trial::proceedNextScene()
{
    static auto_ptr<Scene> apScene;
    Scene *pScene;

    switch(this->currState)
    {
        case IDLE:
            apScene.reset(new PreTrialScene());
            pScene = apScene.get();        
            this->currState = PRE_TRIAL_SCENE;
            pScene->startScene();
            break;
        case PRE_TRIAL_SCENE:
            this->currState = MAIN_SCENE;
            // TODO: Conduct the trial
            break;
        case MAIN_SCENE:
            //TODO: Finish the trial
            break;
        default:
            break;
    }
    return TRUE;
}

BOOL Trial::stepTrial()
{
    return TRUE;
}

auto_ptr<Trial> Trial::m_pInstance;

Trial *Trial::getInstance()
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new Trial);
    }

    return m_pInstance.get();
}
