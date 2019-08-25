#include "common.h"
#include "unregister.h"
#include "util.h"
#include "resource.h"

//関連づけとシェル拡張を解除するための設定ファイルを書く
void WriteUnregisterScript(LPCTSTR strIniName,bool bAssoc)
{
	//関連付け削除
	LPCTSTR extArray[]={	//関連づけ対象となる拡張子の一覧;セクション解析が面倒なので。
		_T(".lzh"),
		_T(".lzs"),
		_T(".lha"),
		_T(".zip"),
		_T(".jar"),
		_T(".cab"),
		_T(".7z"),
		_T(".arj"),
		_T(".rar"),
		_T(".jak"),
		_T(".gca"),
		_T(".imp"),
		_T(".ace"),
		_T(".yz1"),
		_T(".hki"),
		_T(".bza"),
		_T(".gza"),
		_T(".ish"),
		_T(".uue"),
		_T(".bel"),
		_T(".tar"),
		_T(".gz"),
		_T(".tgz"),
		_T(".bz2"),
		_T(".tbz"),
		_T(".xz"),
		_T(".txz"),
		_T(".lzma"),
		_T(".tlz"),
		_T(".z"),
		_T(".taz"),
		_T(".cpio"),
		_T(".a"),
		_T(".lib"),
		_T(".rpm"),
		_T(".deb"),
		_T(".iso"),
	};
	const UINT extCount=sizeof(extArray)/sizeof(extArray[0]);

	if(bAssoc){	//bAssoc=trueのときのみ関連づけを削除
		for(UINT i=0;i<extCount;i++){
			WritePrivateProfileString(extArray[i],_T("set"),_T("0"),strIniName);
		}
	}

	//---シェル解除要請
	WritePrivateProfileString(_T("Shell"),_T("set"),_T("0"),strIniName);
}


//呼び出し
void CallAssistant(LPCTSTR strIniName)
{
	//64bitなら64bit専用処理を先に走らせる
	if(UtilIsWow64()){
		//先にLFAssist(64bit)に処理を渡す。ただしINI削除は行わない。
		//---アシスタント(64bit)のパスを取得
		CPath strExePath(UtilGetModuleDirectoryPath());
		strExePath+=_T("LFAssist64.exe");
		if(strExePath.FileExists()){	//ファイルが存在するときのみ
			strExePath.QuoteSpaces();

			//---実行:CreateProcessではUACをチェックして実行してもらえない
			SHELLEXECUTEINFO shei={0};
			shei.fMask=SEE_MASK_FLAG_DDEWAIT;	//瞬時に終了する可能性があるのでこれを指定する
			shei.cbSize=sizeof(shei);
			shei.lpFile=strExePath;
			shei.lpParameters=strIniName;
			shei.nShow=SW_SHOW;
			if(!ShellExecuteEx(&shei)){
				//実行エラー
				::MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_ERROR_CANNOT_EXECUTE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
			}
		}
	}

	//LFAssist(32bit)にINI削除を要請
	WritePrivateProfileString(_T("PostProcess"),_T("DeleteMe"),_T("Please_Delete_Me"),strIniName);

	//変更を実行
	//---アシスタントのパスを取得
	CPath strExePath(UtilGetModuleDirectoryPath());
	strExePath+=_T("LFAssist.exe");
	strExePath.QuoteSpaces();

	//---実行:CreateProcessではUACをチェックして実行してもらえない
	SHELLEXECUTEINFO shei={0};
	shei.fMask=SEE_MASK_FLAG_DDEWAIT;	//瞬時に終了する可能性があるのでこれを指定する
	shei.cbSize=sizeof(shei);
	shei.lpFile=strExePath;
	shei.lpParameters=strIniName;
	shei.nShow=SW_SHOW;
	if(!ShellExecuteEx(&shei)){
		//実行エラー
		::MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_ERROR_CANNOT_EXECUTE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
	}else{
		Sleep(100);
		::SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,NULL,NULL);	//関連付けの変更をシェルに通知(変更されていないかもしれないが簡便のため)
	}
}

void UnregisterLhaForge(bool bDeleteAssoc)
{
	//テンポラリINIファイル名取得
	TCHAR szIniName[_MAX_PATH+1]={0};
	UtilGetTemporaryFileName(szIniName,_T("lhf"));

	//書き出し
	WriteUnregisterScript(szIniName,bDeleteAssoc);
	//実行
	CallAssistant(szIniName);
}
