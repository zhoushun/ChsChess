// ChsChess.h : ChsChess Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CChsChessApp:
// �йش����ʵ�֣������ ChsChess.cpp
//

class CChsChessApp : public CWinApp
{
public:
	CChsChessApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChsChessApp theApp;