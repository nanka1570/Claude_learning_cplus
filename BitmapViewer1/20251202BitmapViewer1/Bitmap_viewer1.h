
// BitmapViewer1.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CBitmapViewer1App:
// このクラスの実装については、BitmapViewer1.cpp を参照してください
//

class CBitmapViewer1App : public CWinApp
{
public:
	CBitmapViewer1App();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CBitmapViewer1App theApp;
