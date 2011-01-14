#include "stdafx.h"
#include "resource.h"
#include "ConfWnd.h"
#include "experiment.h"
#include <Windowsx.h>
#include <sstream>
#include <fstream>

auto_ptr<ConfWnd> ConfWnd::m_pInstance;

const int ConfWnd::ruleKeywordsNo = 3;
const string ConfWnd::ruleKeywords[] = {"/SID/", "/Date/", "/Time/"};

ConfWnd::ConfWnd(void)
{

}

ConfWnd::~ConfWnd(void)
{

}

ConfWnd* ConfWnd::getInstance()
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new ConfWnd);
    }

    return m_pInstance.get();
}

void ConfWnd::updateTrialNo(HWND hDlg)
{
    unsigned int secNo, trialsInOneSec, trials;

    secNo = GetDlgItemInt(hDlg, IDC_EDIT2, 0, FALSE);
    trialsInOneSec = GetDlgItemInt(hDlg, IDC_EDIT1, 0, FALSE);

    trials = secNo * trialsInOneSec;

    SetDlgItemInt(hDlg, IDC_EDIT3, trials, FALSE);

    return;
}

void ConfWnd::updateOutputFilename(HWND hDlg)
{
    // Get the subject ID
    unsigned int uiSubjectIDLen;
    HWND hSubjectIDEditBox;

    hSubjectIDEditBox = GetDlgItem(hDlg, IDC_EDIT4);
    uiSubjectIDLen = SendMessage(hSubjectIDEditBox, EM_LINELENGTH, 0, 0);

    char *lpstrSubjectID = new char [uiSubjectIDLen + 10];
    GetDlgItemText(hDlg, IDC_EDIT4, (LPSTR)lpstrSubjectID, uiSubjectIDLen + 1);
    string strSubjectID(lpstrSubjectID);
    delete [] lpstrSubjectID;

    // Get the output filename rule
    unsigned int uiRuleLen;
    HWND hRuleEditBox;

    hRuleEditBox = GetDlgItem(hDlg, IDC_EDIT5);
    uiRuleLen = SendMessage(hRuleEditBox, EM_LINELENGTH, 0, 0);

    char *lpstrRule = new char[uiRuleLen + 10];

    GetDlgItemText(hDlg, IDC_EDIT5, (LPSTR)lpstrRule, uiRuleLen + 1);

    string strRule(lpstrRule);
    delete [] lpstrRule;


    // Generate output filename
    int startPos, endPos, prevPos;
    int foundKeyword = 0;
    string outputFilename;
    SYSTEMTIME *p_st = new SYSTEMTIME;

    GetLocalTime(p_st);

    ostringstream ossDate;
    ossDate << p_st->wYear << '-' << p_st->wMonth << '-' << p_st->wDay;

    ostringstream ossTime;
    ossTime << p_st->wHour << '-' << p_st->wMinute;

    this->strDate = ossDate.str();
    this->strTime = ossTime.str();

    prevPos = 0;
    startPos = strRule.find('/');
    while(startPos != string::npos &&
            (endPos = strRule.find('/', startPos + 1)) != string::npos)
    {
        foundKeyword = 0;
        outputFilename += strRule.substr(prevPos, startPos - prevPos);

        for(int i = 0; i < ConfWnd::ruleKeywordsNo; i ++ )
        {
            int ret; 
            ret = strRule.compare(startPos, endPos - startPos + 1, ConfWnd::ruleKeywords[i]);

            if(!ret)
            {
                foundKeyword = 1;

                switch(i)
                {
                    // the Keyword is "/SID/"
                    case 0:
                        {
                            outputFilename += strSubjectID;
                            break;
                        }
                        // the Keyword is "/Date/"
                    case 1:
                        {
                            outputFilename += this->strDate;
                            break;
                        }
                        // the Keyword is "/Time/"
                    case 2:
                        {
                            outputFilename += this->strTime;
                            break;
                        }
                }
                break;
            }
        }

        // If we do not find keyword, then copy the part wrapped in the '/' directly to the output filename
        if(!foundKeyword)
        {
            prevPos = startPos;
            startPos = endPos;
        }
        // If we find any keyword, copy the part from the end of the keyword to the next '/' to the output filename
        else
        {
            prevPos = endPos + 1;
            startPos = strRule.find('/', endPos + 1);
        }

    }

    outputFilename += strRule.substr(prevPos);


    SetDlgItemText(hDlg, IDC_EDIT6, (LPCSTR)outputFilename.c_str());

    delete p_st;

    return;
}

int ConfWnd::validateBlanks(HWND hDlg)
{
    // validate Total number of trials > 0
    unsigned int totalTrials;
    BOOL inExperimentMode = FALSE;
    UINT ret;

    ret = IsDlgButtonChecked(hDlg, IDC_RADIO1);
    if(ret == BST_CHECKED)
    {
        inExperimentMode = TRUE;
    }
    else
    {
        inExperimentMode = FALSE;
    }

    totalTrials = GetDlgItemInt(hDlg, IDC_EDIT3, 0, FALSE);

    // validate Output filename != "" (if in experiment mode)

    unsigned int uiOutputFilenameLen;
    HWND hOutputFilenameEditBox;

    if(inExperimentMode)
    {
        hOutputFilenameEditBox = GetDlgItem(hDlg, IDC_EDIT6);
        uiOutputFilenameLen = SendMessage(hOutputFilenameEditBox, EM_LINELENGTH, 0, 0);
    }
    if(totalTrials <= 0)
    {
        //TODO: MARK corresponding items red 
    }

    if(inExperimentMode && uiOutputFilenameLen <= 0)
    {
        //TODO: MARK corresponding items red 
    }

    if(totalTrials <= 0 || (inExperimentMode &&uiOutputFilenameLen <= 0))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int ConfWnd::validateFileExistance(HWND hDlg)
{
    // Check the mode, experiment mode or practice mode
    BOOL inExperimentMode = FALSE;
    UINT ret;

    ret = IsDlgButtonChecked(hDlg, IDC_RADIO1);
    if(ret == BST_CHECKED)
    {
        inExperimentMode = TRUE;
    }
    else
    {
        inExperimentMode = FALSE;
    }

    if(!inExperimentMode)
        return 0;


    // Check if the file is existed
    
    // get output filename
    unsigned int uiOutputFilenameLen;
    HWND hOutputFilenameEditBox;

    hOutputFilenameEditBox = GetDlgItem(hDlg, IDC_EDIT6);
    uiOutputFilenameLen = SendMessage(hOutputFilenameEditBox, EM_LINELENGTH, 0, 0);

    char *lpstrOutputFilename = new char[uiOutputFilenameLen + 10];
    GetDlgItemText(hDlg, IDC_EDIT6, (LPSTR)lpstrOutputFilename, uiOutputFilenameLen + 1);

    // try open the file
    ifstream fin(lpstrOutputFilename);
    delete [] lpstrOutputFilename;

    if(fin.good())
    {
        fin.close();
        return -1;
    }
    else
    {
        return 0;
    }
}

void ConfWnd::confirmConfiguration(HWND hDlg)
{
    ConfWnd *pConfWnd = ConfWnd::getInstance();

    // Get the subject ID
    unsigned int uiSubjectIDLen;
    HWND hSubjectIDEditBox;

    hSubjectIDEditBox = GetDlgItem(hDlg, IDC_EDIT4);
    uiSubjectIDLen = SendMessage(hSubjectIDEditBox, EM_LINELENGTH, 0, 0);

    char *lpstrSubjectID = new char [uiSubjectIDLen + 10];
    GetDlgItemText(hDlg, IDC_EDIT4, (LPSTR)lpstrSubjectID, uiSubjectIDLen + 1);
    pConfWnd->subjectID = string(lpstrSubjectID);
    delete [] lpstrSubjectID;

    // store the max section number
    pConfWnd->maxSecNo = GetDlgItemInt(hDlg, IDC_EDIT2, 0, FALSE);

    // Store the number of trials in one section
    pConfWnd->trialsInOneSec = GetDlgItemInt(hDlg, IDC_EDIT1, 0, FALSE);

    // Store the display mode
    HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
    int iMode = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
    iMode = SendMessage(hCombo, CB_GETITEMDATA, iMode, 0);
    EnumDisplaySettings(NULL, iMode, &pConfWnd->devMode);

    // Store the experiment mode
    UINT ret;

    ret = IsDlgButtonChecked(hDlg, IDC_RADIO1);
    if(ret == BST_CHECKED)
    {
        pConfWnd->experiMode = EXPERIMENT;
    }
    else
    {
        pConfWnd->experiMode = PRACTICE;
    }

    // If in experiment mode, store the output filename
    if(pConfWnd->experiMode == 0)
    {
        // get output filename
        unsigned int uiOutputFilenameLen;
        HWND hOutputFilenameEditBox;

        hOutputFilenameEditBox = GetDlgItem(hDlg, IDC_EDIT6);
        uiOutputFilenameLen = SendMessage(hOutputFilenameEditBox, EM_LINELENGTH, 0, 0);

        char *lpstrOutputFilename = new char[uiOutputFilenameLen + 10];
        GetDlgItemText(hDlg, IDC_EDIT6, (LPSTR)lpstrOutputFilename, uiOutputFilenameLen + 1);
        
        pConfWnd->outFilename = string(lpstrOutputFilename);

        delete [] lpstrOutputFilename;
    }

    return;
}

INT_PTR ConfWnd::displayConfWnd(HINSTANCE ghInstance)
{
    INT_PTR ret;

    ret = DialogBox(ghInstance, MAKEINTRESOURCE(IDD_CONFWND), GetDesktopWindow(), confWndProc);

    return ret;
}

INT_PTR CALLBACK ConfWnd::confWndProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    ConfWnd *pConfWnd = ConfWnd::getInstance();

    switch(message)
    {
        // This will be executed when the configure window is initializing
        case WM_INITDIALOG:
            {
                // Set the default values to blanks
                SetDlgItemInt(hDlg, IDC_EDIT2, 0, FALSE); // Number of section
                SetDlgItemInt(hDlg, IDC_EDIT1, 0, FALSE); // Number of trials in one section
                SetDlgItemText(hDlg, IDC_EDIT4, (LPCSTR)("0")); // Subject ID
                SetDlgItemText(hDlg, IDC_EDIT5, (LPCSTR)("/SID/-/Date/-/Time/.txt")); // Output Filename Rule

                // 1. Update total section numbers
                // 2. Update output filename
                pConfWnd->updateTrialNo(hDlg);
                pConfWnd->updateOutputFilename(hDlg);

                // Set the default values to the experiment mode
                CheckDlgButton(hDlg, IDC_RADIO1, BST_CHECKED);

                // Enumerate display modes (Copied from SphereWorld32.cpp)
                unsigned int iMode;
                unsigned int nWidth;
                unsigned int nHeight;
                HWND hCombo;
                DEVMODE devMode;

                iMode = 0;
                nWidth = GetSystemMetrics(SM_CXSCREEN);
                nHeight = GetSystemMetrics(SM_CYSCREEN);
                hCombo = GetDlgItem(hDlg, IDC_COMBO1);
                while(EnumDisplaySettings(NULL, iMode, &devMode))
                {
                    int iItem;
                    ostringstream outstr;

                    outstr << devMode.dmPelsWidth << " x " << devMode.dmPelsHeight << " x ";
                    outstr << devMode.dmBitsPerPel << "bpp @" << devMode.dmDisplayFrequency << "hz\0";

                    string cBuffer = outstr.str();

                    iItem = SendMessage(hCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(LPCTSTR(cBuffer.c_str())));

                    SendMessage(hCombo, CB_SETITEMDATA, iItem, iMode);

                    if(devMode.dmPelsHeight == nHeight &&
                            devMode.dmPelsWidth == nWidth)
                    {
                        SendMessage(hCombo, CB_SETCURSEL, iItem, 0);
                    }

                    iMode ++;
                }

                return TRUE;
            }
            // This will be executed when some actions happens on the window
        case WM_COMMAND:
            {
                switch(LOWORD(wParam))
                {
                    // Press the Confirm button
                    case IDOK:
                        {
                            BOOL succeed = TRUE;

                            // Validate the blanks
                            if(pConfWnd->validateBlanks(hDlg) != 0)
                            {
                                MessageBox(NULL, (LPCSTR)"Some blanks are invalid, and please check.\n"
                                        "Notice that the total number of trials cannot be zero, and "
                                        "the output filename cannot be nothing.", (LPCSTR)"Configuration Warning!",
                                        MB_OK | MB_ICONERROR);
                                succeed = FALSE;
                            }

                            if(succeed == FALSE)
                                break;

                            // Validate if the output file is existed
                            if(pConfWnd->validateFileExistance(hDlg) != 0)
                            {
                                int ret;
                                ret = MessageBox(NULL, (LPCSTR)"The chosen output file is existed."
                                    " Choose another filename please.", NULL, MB_OK | MB_ICONERROR);
                                succeed = FALSE;
                            }
                        
                            if(succeed)
                            {
                                // store configurations in ConfWnd
                                pConfWnd->confirmConfiguration(hDlg);
                                EndDialog(hDlg, TRUE);
                            }
                            break;
                        }
                    // Press the exit button
                    case IDEXIT:
                        {
                            EndDialog(hDlg, FALSE);
                            break;
                        }
                    case IDC_EDIT2:
                        {
                            // If the number of section changed, 
                            // re-calculate the total number of the trials
                            if(HIWORD(wParam) == EN_CHANGE)
                            {
                                pConfWnd->updateTrialNo(hDlg);
                            }
                            break;
                        }
                    case IDC_EDIT1:
                        {
                            // If the number of trials in each section
                            // changed, recalculate the total number of the trials
                            if(HIWORD(wParam) == EN_CHANGE)
                            {
                                pConfWnd->updateTrialNo(hDlg);
                            }
                            break;
                        }
                    case IDC_EDIT4:
                        {
                            // If the subject ID changed, update 
                            // the output filename
                            if(HIWORD(wParam) == EN_CHANGE)
                            {
                                pConfWnd->updateOutputFilename(hDlg);
                            }
                            break;
                        }
                    case IDC_EDIT5:
                        {
                            // If the output filename generation rule changed,
                            // update the output filename
                            if(HIWORD(wParam) == EN_CHANGE)
                            {
                                pConfWnd->updateOutputFilename(hDlg);
                            }
                            break;
                        }
                    case IDC_RADIO1:
                        {
                            // If select experiment mode, update the output filename and enable related blanks
                            if(HIWORD(wParam) == BN_CLICKED)
                            {
                                HWND hSubjectIDEditBox, hOutputFilenameRuleEditBox, hOutputFilenameEditBox;
                                hSubjectIDEditBox = GetDlgItem(hDlg, IDC_EDIT4);
                                hOutputFilenameRuleEditBox = GetDlgItem(hDlg, IDC_EDIT5);
                                hOutputFilenameEditBox = GetDlgItem(hDlg, IDC_EDIT6);

                                Edit_Enable(hSubjectIDEditBox, TRUE);
                                Edit_Enable(hOutputFilenameRuleEditBox, TRUE);
                                Edit_Enable(hOutputFilenameEditBox, TRUE);

                                pConfWnd->updateOutputFilename(hDlg);
                            }
                            break;
                        }
                    case IDC_RADIO2:
                        {
                            // If select practice mode, disable output filename related blanks
                            if(HIWORD(wParam) == BN_CLICKED)
                            {
                                HWND hSubjectIDEditBox, hOutputFilenameRuleEditBox, hOutputFilenameEditBox;
                                hSubjectIDEditBox = GetDlgItem(hDlg, IDC_EDIT4);
                                hOutputFilenameRuleEditBox = GetDlgItem(hDlg, IDC_EDIT5);
                                hOutputFilenameEditBox = GetDlgItem(hDlg, IDC_EDIT6);

                                Edit_Enable(hSubjectIDEditBox, FALSE);
                                Edit_Enable(hOutputFilenameRuleEditBox, FALSE);
                                Edit_Enable(hOutputFilenameEditBox, FALSE);
                            }
                            break;
                        }
                }
                break;
            }
            // This will be executed when the configuration window is closing
        case WM_CLOSE:
            {
                EndDialog(hDlg, FALSE);
                break;
            }
    }

    return FALSE;
}
