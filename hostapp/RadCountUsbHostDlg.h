// RadCountUsbHostDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_)
#define AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRadCountUsbHostDlg �_�C�A���O

class CRadCountUsbHostDlg : public CDialog
{
// �\�z
public:
	CRadCountUsbHostDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRadCountUsbHostDlg)
	enum { IDD = IDD_RADCOUNTUSBHOST_DIALOG };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRadCountUsbHostDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RADCOUNTUSBHOSTDLG_H__BA1B6767_7301_4633_933F_CAC840ADA288__INCLUDED_)
