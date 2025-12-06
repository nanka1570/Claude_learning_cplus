
// MFCPracticeByClaude.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMFCPracticeByClaudeApp:
// このクラスの実装については、MFCPracticeByClaude.cpp を参照してください
//

class CMFCPracticeByClaudeApp : public CWinApp
{
public:
	CMFCPracticeByClaudeApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMFCPracticeByClaudeApp theApp;
