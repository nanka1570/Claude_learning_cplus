
// Bitmap_viewer2.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CBitmapviewer2App:
// このクラスの実装については、Bitmap_viewer2.cpp を参照してください
//

class CBitmapviewer2App : public CWinApp
{
public:
	CBitmapviewer2App();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CBitmapviewer2App theApp;
