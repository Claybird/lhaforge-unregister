#include "common.h"
#include "unregister.h"
#include "resource.h"
#include "util.h"

//アンインストーラ

CAppModule _Module;

//エントリーポイント
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG)
	// メモリリーク検出用
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF
		| _CRTDBG_LEAK_CHECK_DF
		);
#endif
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	// これはMicrosoft Layer for Unicode (MSLU) が使用された時の
	// ATLウインドウ thunking 問題を解決する
	::DefWindowProc(NULL, 0, 0, 0L);

	// 他のコントロールをサポートするためのフラグを追加
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);
	_Module.Init(NULL,hInstance);

	//コマンドライン引数の取得
	std::vector<CString> Params;
	UtilGetCommandLineParams(Params);
	if(Params.size()>=2){
//セットアップモード
		if(_T("/setup")==Params[1]){//セットアップモードなので、アンインストール情報を書き換える
			//アンインストール情報のキーを開く
			//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LhaForge
			HKEY hKey;
			LONG Ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\LhaForge"), NULL,KEY_ALL_ACCESS, &hKey);
			if(ERROR_SUCCESS!=Ret){
				return 1;
			}
			//自分の情報に書き換え
			TCHAR ExePath[_MAX_PATH+1];
			ZeroMemory(&ExePath,sizeof(ExePath));
			GetModuleFileName(GetModuleHandle(NULL), ExePath, _MAX_PATH);
			DWORD dwWrite=(_tcslen(ExePath)+1)*sizeof(TCHAR);
			::RegSetValueEx(hKey, _T("UninstallString"), NULL, REG_SZ, (BYTE*)ExePath,dwWrite);
			::RegCloseKey(hKey);
		}
	}else{
		//アンインストールモード

		//アンインストールを開始するかどうか
		if(IDYES==MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_MESSAGE_CONTINUE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_YESNO|MB_ICONQUESTION)){
			//関連付けを削除するかどうか
			bool bDeleteAssoc=(IDYES==MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_DELETE_ASSOC)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_YESNO|MB_ICONQUESTION));
			//シェル拡張は強制解除
			UnregisterLhaForge(bDeleteAssoc);

			//ショートカット削除のための云々
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
			// アンインストーラのパス取得
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

