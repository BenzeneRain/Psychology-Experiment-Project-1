#include "stdafx.h"
#include "Trial.h"
#include "Scene.h"
#include "PreTrialScene.h"
#include "Separate2D3DViewScene.h"

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
    return this->proceedNextScene();
}

BOOL Trial::proceedNextScene()
{
    static auto_ptr<Scene> apScene;
    Scene *pScene;
    BOOL ret;

    switch(this->currState)
    {
        case IDLE:
            apScene.reset(new PreTrialScene());
            pScene = apScene.get();        
            this->currState = PRE_TRIAL_SCENE;
            ret = pScene->startScene();
            break;
        case PRE_TRIAL_SCENE:
            apScene.reset(new Separate2D3DViewScene());
            pScene = apScene.get();        
            this->currState = MAIN_SCENE;
            ret = pScene->startScene();
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
