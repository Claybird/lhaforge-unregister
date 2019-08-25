#include "common.h"
#include "unregister.h"
#include "util.h"
#include "resource.h"

//�֘A�Â��ƃV�F���g�����������邽�߂̐ݒ�t�@�C��������
void WriteUnregisterScript(LPCTSTR strIniName,bool bAssoc)
{
	//�֘A�t���폜
	LPCTSTR extArray[]={	//�֘A�Â��ΏۂƂȂ�g���q�̈ꗗ;�Z�N�V������͂��ʓ|�Ȃ̂ŁB
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

	if(bAssoc){	//bAssoc=true�̂Ƃ��̂݊֘A�Â����폜
		for(UINT i=0;i<extCount;i++){
			WritePrivateProfileString(extArray[i],_T("set"),_T("0"),strIniName);
		}
	}

	//---�V�F�������v��
	WritePrivateProfileString(_T("Shell"),_T("set"),_T("0"),strIniName);
}


//�Ăяo��
void CallAssistant(LPCTSTR strIniName)
{
	//64bit�Ȃ�64bit��p�������ɑ��点��
	if(UtilIsWow64()){
		//���LFAssist(64bit)�ɏ�����n���B������INI�폜�͍s��Ȃ��B
		//---�A�V�X�^���g(64bit)�̃p�X���擾
		CPath strExePath(UtilGetModuleDirectoryPath());
		strExePath+=_T("LFAssist64.exe");
		if(strExePath.FileExists()){	//�t�@�C�������݂���Ƃ��̂�
			strExePath.QuoteSpaces();

			//---���s:CreateProcess�ł�UAC���`�F�b�N���Ď��s���Ă��炦�Ȃ�
			SHELLEXECUTEINFO shei={0};
			shei.fMask=SEE_MASK_FLAG_DDEWAIT;	//�u���ɏI������\��������̂ł�����w�肷��
			shei.cbSize=sizeof(shei);
			shei.lpFile=strExePath;
			shei.lpParameters=strIniName;
			shei.nShow=SW_SHOW;
			if(!ShellExecuteEx(&shei)){
				//���s�G���[
				::MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_ERROR_CANNOT_EXECUTE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
			}
		}
	}

	//LFAssist(32bit)��INI�폜��v��
	WritePrivateProfileString(_T("PostProcess"),_T("DeleteMe"),_T("Please_Delete_Me"),strIniName);

	//�ύX�����s
	//---�A�V�X�^���g�̃p�X���擾
	CPath strExePath(UtilGetModuleDirectoryPath());
	strExePath+=_T("LFAssist.exe");
	strExePath.QuoteSpaces();

	//---���s:CreateProcess�ł�UAC���`�F�b�N���Ď��s���Ă��炦�Ȃ�
	SHELLEXECUTEINFO shei={0};
	shei.fMask=SEE_MASK_FLAG_DDEWAIT;	//�u���ɏI������\��������̂ł�����w�肷��
	shei.cbSize=sizeof(shei);
	shei.lpFile=strExePath;
	shei.lpParameters=strIniName;
	shei.nShow=SW_SHOW;
	if(!ShellExecuteEx(&shei)){
		//���s�G���[
		::MessageBox(NULL,CString(MAKEINTRESOURCE(IDS_ERROR_CANNOT_EXECUTE)),CString(MAKEINTRESOURCE(IDS_MESSAGE_CAPTION)),MB_OK|MB_ICONSTOP);
	}else{
		Sleep(100);
		::SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,NULL,NULL);	//�֘A�t���̕ύX���V�F���ɒʒm(�ύX����Ă��Ȃ���������Ȃ����ȕւ̂���)
	}
}

void UnregisterLhaForge(bool bDeleteAssoc)
{
	//�e���|����INI�t�@�C�����擾
	TCHAR szIniName[_MAX_PATH+1]={0};
	UtilGetTemporaryFileName(szIniName,_T("lhf"));

	//�����o��
	WriteUnregisterScript(szIniName,bDeleteAssoc);
	//���s
	CallAssistant(szIniName);
}
