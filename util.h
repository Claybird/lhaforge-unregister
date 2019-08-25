//LhaForge Unregister用

//デバッグ用関数および便利な関数群
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
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
		(LPTSTR)&lpMsgBuf, 0, NULL);
	TRACE(_T("API Error : %s"),lpMsgBuf);
	LocalFree(lpMsgBuf);
}

#else
// Releaseのとき
#define TraceLastError
#define TRACE

#endif	//_DEBUG

//=============================================
// 共通便利関数
//=============================================

bool UtilGetTemporaryFileName(LPTSTR fname,LPCTSTR prefix);

//WindowsNTかどうか
bool UtilIsWindowsNT();
//自分のプログラムのファイル名を返す
LPCTSTR UtilGetModulePath();
//自分のプログラムのおいてあるディレクトリのパス名を返す
LPCTSTR UtilGetModuleDirectoryPath();

//コマンドライン引数を取得(個数を返す)
int UtilGetCommandLineParams(std::vector<CString>&);

//現在のOSがWindowsVistaもしくはそれ以上ならtrueを返す
bool UtilIsOSVistaOrHigher();

//WoW64(64bit OSでの32bitエミュレーション)で動いていればTRUEを返す関数
BOOL UtilIsWow64();
