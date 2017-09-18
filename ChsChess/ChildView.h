// ChildView.h : CChildView 类的接口
//


#pragma once
#include "afxwin.h"

struct GRIDPOINT {
   int row;
   int col;
};

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 属性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
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

