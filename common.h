//���ʂ���include���ׂ��t�@�C�������ׂď����Ă���

#pragma once
#define _WIN32_WINNT 0x0600
#define ATL_NO_LEAN_AND_MEAN
//ATL��CString���g��
#define _WTL_NO_CSTRING
#define _STLP_USE_NEWALLOC	//STL�ŕW���̃A���P�[�^���g��

#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <list>
#include <vector>
#include <atlpath.h>
