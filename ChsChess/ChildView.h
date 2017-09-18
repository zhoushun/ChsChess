// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "afxwin.h"

struct GRIDPOINT {
   int row;
   int col;
};

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_bmBoard;
	CBitmap m_bmPieces;
	CBitmap m_bmSelect;
	int m_BoardWidth;
	int m_BoardHeight;
	int m_gameState;
	int m_turn;

public:
	void DrawPiece(CDC* pDC, int nRow, int col, int piece);
	void DrawPieces(CDC* pDC);
	void DrawBoard(CDC* pDC);
	void DrawSelect(CDC* pDC, int nRow, int col);

	void InitGame(void);
	bool isRed(int piece);
	bool isBlack(int piece);
	bool isPlayerSide(int player, int piece);
	bool InBound(int nRow, int col);
	bool isSameSide(int nRowSrc, int nColSrc, int nRowDest, int nColDest);
	bool isValidMove(int nRowSrc, int nColSrc, int nRowDest, int nColDest);
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	GRIDPOINT m_lastSelPoint;
	GRIDPOINT m_SelPoint;
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);


	void GenRMove(int nRow, int nCol);
	void GenHMove(int nRow, int nCol);
	void GenCMove(int nRow, int nCol);
	void GenKMove(int nRow, int nCol);
	void GenAMove(int nRow, int nCol);
	void GenEMove(int nRow, int nCol);
	void GenPMove(int nRow, int nCol);
public:
	void AddMove(int nRow, int nCol);
	void DrawMove(CDC* pDC);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

