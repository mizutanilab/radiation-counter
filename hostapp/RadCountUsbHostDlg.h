// RadCountUsbHostDlg.h : ヘッダー ファイル
//

#if !defined(AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_)
#define AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRadCountUsbHostDlg ダイアログ

class CRadCountUsbHostDlg : public CDialog
{
// 構築
public:
	CRadCountUsbHostDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRadCountUsbHostDlg)
	enum { IDD = IDD_RADCOUNTUSBHOST_DIALOG };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRadCountUsbHostDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRadCountUsbHostDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_)
