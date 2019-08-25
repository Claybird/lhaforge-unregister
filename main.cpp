#include "common.h"
#include "unregister.h"
#include "resource.h"
#include "util.h"

//�A���C���X�g�[��

CAppModule _Module;

//�G���g���[�|�C���g
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG)
	// ���������[�N���o�p
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF
		| _CRTDBG_LEAK_CHECK_DF
		);
#endif
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	// �����Microsoft Layer for Unicode (MSLU) ���g�p���ꂽ����
	// ATL�E�C���h�E thunking ������������
	::DefWindowProc(NULL, 0, 0, 0L);

	// ���̃R���g���[�����T�|�[�g���邽�߂̃t���O��ǉ�
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);
	_Module.Init(NULL,hInstance);

	//�R�}���h���C�������̎擾
	std::vector<CString> Params;
	UtilGetCommandLineParams(Params);
	if(Params.size()>=2){
//�Z�b�g�A�b�v���[�h
		if(_T("/setup")==Params[1]){//�Z�b�g�A�b�v���[�h�Ȃ̂ŁA�A���C���X�g�[����������������
			//�A���C���X�g�[�����̃L�[���J��
			//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LhaForge
			HKEY hKey;
			LONG Ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\LhaForge"), NULL,KEY_ALL_ACCESS, &hKey);
			if(ERROR_SUCCESS!=Ret){
				return 1;
			}
			//�����̏��ɏ�������
			TCHAR ExePath[_MAX_PATH+1];
			ZeroMemory(&ExePath,sizeof(ExePath));
			GetModuleFileName(GetModuleHandle(NULL), ExePath, _MAX_PATH);
			DWORD dwWrite=(_tcslen(ExePath)+1)*sizeof(TCHAR);
			::RegSetValueEx(hKey, _T("UninstallString"), NULL, REG_SZ, (BYTE*)ExePath,dwWrite);
			::RegCloseKey(hKey);
		}
	}else{
		//�A���C���X�g�[�����[�h

		//�A���C���X�g�[�����J�n���邩�ǂ���
		if(IDYES==MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_MESSAGE_CONTINUE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_YESNO|MB_ICONQUESTION)){
			//�֘A�t�����폜���邩�ǂ���
			bool bDeleteAssoc=(IDYES==MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_DELETE_ASSOC)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_YESNO|MB_ICONQUESTION));
			//�V�F���g���͋�������
			UnregisterLhaForge(bDeleteAssoc);

			//�V���[�g�J�b�g�폜�̂��߂̉]�X
			if(IDYES==MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_OPEN_SENDTO)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_YESNO|MB_ICONQUESTION)){
				TCHAR SendToPath[_MAX_PATH+1];
				if(!SHGetSpecialFolderPath(NULL,SendToPath,CSIDL_SENDTO,FALSE)){
					MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_FAILED_TO_GET_SENDTO)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
				}
				else{
					ShellExecute(NULL, _T("open"), SendToPath, NULL, NULL, SW_SHOWNORMAL);
				}
			}
			//----------------------------
			// �A���C���X�g�[���̃p�X�擾
			//----------------------------
			TCHAR ExePath[_MAX_PATH+1];
			ZeroMemory(&ExePath,sizeof(ExePath));
			GetModuleFileName(GetModuleHandle(NULL), ExePath, _MAX_PATH);

			PathRemoveFileSpec(ExePath);
			PathAppend(ExePath,_T("epuninst.exe"));

			if(32>=(int)ShellExecute(NULL,NULL,ExePath,_T("/s"),NULL,SW_RESTORE)){
				MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_ERROR_CANNOT_EXECUTE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
			}
		}
	}
	_Module.Term();
	::CoUninitialize();
	return 0;
}

