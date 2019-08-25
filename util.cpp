#include "common.h"
#include "util.h"

void UtilDebugTrace(LPCTSTR pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);
	CString str;
	str.FormatV(pszFormat,args);
	va_end(args);

	OutputDebugString(str);
}

bool UtilGetTemporaryFileName(LPTSTR fname,LPCTSTR prefix)
{
	TCHAR TempDirPath[_MAX_PATH+1]={0};
	GetTempPath(_MAX_PATH,TempDirPath);
	if(!GetTempFileName(TempDirPath,prefix,0,fname))
	{
		//ErrorMessage(CString(MAKEINTRESOURCE(IDS_ERROR_TEMPORARY_FILE_CREATE)));
		return false;
	}
	return true;
}


bool UtilIsWindowsNT()
{
	OSVERSIONINFO osi={0};
	osi.dwOSVersionInfoSize=sizeof(osi);
	GetVersionEx(&osi);
	return (VER_PLATFORM_WIN32_NT==osi.dwPlatformId);
}


//�����̃v���O�����̃t�@�C������Ԃ�
LPCTSTR UtilGetModulePath()
{
	static TCHAR s_szExePath[_MAX_PATH+1]={0};
	if(s_szExePath[0]!=_T('\0'))return s_szExePath;

	GetModuleFileName(GetModuleHandle(NULL), s_szExePath, _MAX_PATH);	//�{�̂̃p�X�擾
	return s_szExePath;
}

//�����̃v���O�����̂����Ă���f�B���N�g���̃p�X����Ԃ�
LPCTSTR UtilGetModuleDirectoryPath()
{
	static TCHAR s_szDirPath[_MAX_PATH+1]={0};
	if(s_szDirPath[0]!=_T('\0'))return s_szDirPath;

	GetModuleFileName(GetModuleHandle(NULL), s_szDirPath, _MAX_PATH);	//�{�̂̃p�X�擾
	PathRemoveFileSpec(s_szDirPath);
	return s_szDirPath;
}


//�R�}���h���C���������擾(����Ԃ�)
int UtilGetCommandLineParams(std::vector<CString> &rParams)
{
#if defined(_UNICODE)||defined(UNICODE)
	int nArgc=0;
	LPWSTR *lplpArgs=CommandLineToArgvW(GetCommandLine(), &nArgc);
	rParams.resize(nArgc);
	for(int i=0;i<nArgc;i++){
		rParams[i]=lplpArgs[i];
	}
	LocalFree(lplpArgs);
	return nArgc;
#else//defined(_UNICODE)||defined(UNICODE)
	rParams.resize(__argc);
	for(int i=0;i<__argc;i++){
		rParams[i]=__argv[i];
	}
	return __argc;
#endif//defined(_UNICODE)||defined(UNICODE)
}


//���݂�OS��WindowsVista�������͂���ȏ�Ȃ�true��Ԃ�
bool UtilIsOSVistaOrHigher()
{
	OSVERSIONINFO osv={0};
	osv.dwOSVersionInfoSize=sizeof(osv);
	if(!::GetVersionEx(&osv)){
		ASSERT(!"GetVersionEx failed");
		return false;
	}
	if(osv.dwMajorVersion>6||(osv.dwMajorVersion==6&&osv.dwMinorVersion>=0))return true;
	return false;
}


BOOL UtilIsWow64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	BOOL bIsWow64 = FALSE;

	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");

	if(fnIsWow64Process){
		if(!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)){
			return FALSE;
		}
	}
	return bIsWow64;
}

