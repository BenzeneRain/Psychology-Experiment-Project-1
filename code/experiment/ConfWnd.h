#ifndef _H_CONFWND_
#define _H_CONFWND_

#include "stdafx.h"
#include <string>
#include <memory>

using namespace std;

// ConfWnd is a singleton
class ConfWnd
{
    public:
        ~ConfWnd();
        static ConfWnd *getInstance();

        INT_PTR displayConfWnd(HINSTANCE ghInstance);
        static INT_PTR CALLBACK confWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);

        static const int ruleKeywordsNo;
        static const string ruleKeywords[];

        string subjectID;
        unsigned int maxSecNo;
        unsigned int experiMode; // 0 -- experiment mode; 1 -- practice mode;
        string outFilename;
        unsigned int trialsInOneSec;
        DEVMODE devMode;

    private:
        ConfWnd();
        void updateTrialNo(HWND hDlg);
        void updateOutputFilename(HWND hDlg);
        int validateBlanks(HWND hDlg);
        int validateFileExistance(HWND hDlg);
        void confirmConfiguration(HWND hDlg);
        
        static auto_ptr<ConfWnd> m_pInstance;
        string m_secNo;

};


#endif /* _H_CONFWND_ */
