
// Bitmap_viewer1.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CBitmapviewer1App:
// このクラスの実装については、Bitmap_viewer1.cpp を参照してください
//

class CBitmapviewer1App : public CWinApp
{
public:
	CBitmapviewer1App();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CBitmapviewer1App theApp;
