
// 2015Remote.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "2015Remote.h"
#include "2015RemoteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// dump���
#include <io.h>
#include <direct.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

/** 
* @brief ��������δ֪BUG������ֹʱ���ô˺�����������
* ����ת��dump�ļ���dumpĿ¼.
*/
long WINAPI whenbuged(_EXCEPTION_POINTERS *excp)
{
	// ��ȡdump�ļ��У��������ڣ��򴴽�֮
	char dump[_MAX_PATH], *p = dump;
	GetModuleFileNameA(NULL, dump, _MAX_PATH);
	while (*p) ++p;
	while ('\\' != *p) --p;
	strcpy(p + 1, "dump");
	if (_access(dump, 0) == -1)
		_mkdir(dump);
	char curTime[64];// ��ǰdump�ļ�
	time_t TIME(time(0));
	strftime(curTime, 64, "\\remote_%Y-%m-%d %H%M%S.dmp", localtime(&TIME));
	strcat(dump, curTime);
	HANDLE hFile = ::CreateFileA(dump, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE != hFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo = {::GetCurrentThreadId(), excp, FALSE};
		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
			hFile, MiniDumpWithFullMemory, &einfo, NULL, NULL);
		::CloseHandle(hFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

// CMy2015RemoteApp

BEGIN_MESSAGE_MAP(CMy2015RemoteApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy2015RemoteApp ����

CMy2015RemoteApp::CMy2015RemoteApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMy2015RemoteApp ����

CMy2015RemoteApp theApp;


// CMy2015RemoteApp ��ʼ��

BOOL CMy2015RemoteApp::InitInstance()
{
	SetUnhandledExceptionFilter(&whenbuged);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Remoter"));

	CMy2015RemoteDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}