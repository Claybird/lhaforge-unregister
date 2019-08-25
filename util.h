//LhaForge Unregister�p

//�f�o�b�O�p�֐�����ѕ֗��Ȋ֐��Q
#pragma once
//#pragma warning(disable:4786)

#define ASSERT(x)	assert(x)

//TRACE
void UtilDebugTrace(LPCTSTR pszFormat, ...);

#if defined(_DEBUG) || defined(DEBUG)

#define TRACE UtilDebugTrace

static void TraceLastError()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
		(LPTSTR)&lpMsgBuf, 0, NULL);
	TRACE(_T("API Error : %s"),lpMsgBuf);
	LocalFree(lpMsgBuf);
}

#else
// Release�̂Ƃ�
#define TraceLastError
#define TRACE

#endif	//_DEBUG

//=============================================
// ���ʕ֗��֐�
//=============================================

bool UtilGetTemporaryFileName(LPTSTR fname,LPCTSTR prefix);

//WindowsNT���ǂ���
bool UtilIsWindowsNT();
//�����̃v���O�����̃t�@�C������Ԃ�
LPCTSTR UtilGetModulePath();
//�����̃v���O�����̂����Ă���f�B���N�g���̃p�X����Ԃ�
LPCTSTR UtilGetModuleDirectoryPath();

//�R�}���h���C���������擾(����Ԃ�)
int UtilGetCommandLineParams(std::vector<CString>&);

//���݂�OS��WindowsVista�������͂���ȏ�Ȃ�true��Ԃ�
bool UtilIsOSVistaOrHigher();

//WoW64(64bit OS�ł�32bit�G�~�����[�V����)�œ����Ă����TRUE��Ԃ��֐�
BOOL UtilIsWow64();
