// RadCountUsbHost.h : RADCOUNTUSBHOST �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_)
#define AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CRadCountUsbHostApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� RadCountUsbHost.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CRadCountUsbHostApp : public CWinApp
{
public:
	CRadCountUsbHostApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRadCountUsbHostApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CRadCountUsbHostApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RADCOUNTUSBHOST_H__67DE5323_EA27_455A_84BC_B11B1A5C91D1__INCLUDED_)
