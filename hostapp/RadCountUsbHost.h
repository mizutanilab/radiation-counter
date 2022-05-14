// RadCountUsbHost.h : RADCOUNTUSBHOST アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_)
#define AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CRadCountUsbHostApp:
// このクラスの動作の定義に関しては RadCountUsbHost.cpp ファイルを参照してください。
//

class CRadCountUsbHostApp : public CWinApp
{
public:
	CRadCountUsbHostApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRadCountUsbHostApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CRadCountUsbHostApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_)
