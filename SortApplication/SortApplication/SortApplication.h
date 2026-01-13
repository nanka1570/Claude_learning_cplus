
// SortApplication.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSortApplicationApp:
// このクラスの実装については、SortApplication.cpp を参照してください
//

class CSortApplicationApp : public CWinApp
{
public:
	CSortApplicationApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSortApplicationApp theApp;
