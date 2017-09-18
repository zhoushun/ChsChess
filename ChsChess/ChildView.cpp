// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "ChsChess.h"
#include "ChildView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum GAMESTATE { GS_WAIT, GS_ACTIVE, GS_GAMEOVER, GS_WIN };

enum TURNSTATE { COMPUTERPLAYER, HUMANPLAYER }; 

enum PIECES { NONE = -1, REDCHARIOT, REDHORSE, REDCANNON, REDKING, REDADVISER, REDELEPHANT, REDPAWN,
			  BLACKCHARIOT, BLACKHORSE, BLACKCANNON, BLACKKING, BLACKADVISER, BLACKELEPHANT, BLACKPAWN};

int Board[10][9] =
{
	{BLACKCHARIOT, BLACKHORSE, BLACKELEPHANT, BLACKADVISER, BLACKKING, BLACKADVISER, BLACKELEPHANT, BLACKHORSE, BLACKCHARIOT},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, BLACKCANNON, NONE, NONE, NONE, NONE, NONE, BLACKCANNON, NONE},
	{BLACKPAWN, NONE, BLACKPAWN, NONE, BLACKPAWN, NONE, BLACKPAWN, NONE, BLACKPAWN},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},

	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{REDPAWN, NONE, REDPAWN, NONE, REDPAWN, NONE, REDPAWN, NONE, REDPAWN},
	{NONE, REDCANNON, NONE, NONE, NONE, NONE, NONE, REDCANNON, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{REDCHARIOT, REDHORSE, REDELEPHANT, REDADVISER, REDKING, REDADVISER, REDELEPHANT, REDHORSE, REDCHARIOT},
};
int ValidMove[10][9];

HCURSOR hCurSorValidPos,hCurSorInvalidPos;

int rowAdjacent[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int colAdjacent[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int rowHorseAdjacent[] = {-2, -2, -1, 1, 2, 2, 1, -1};
int colHorseAdjacent[] = {-1, 1, 2, 2, 1, -1, -2, -2};


// CChildView

CChildView::CChildView()
: m_BoardWidth(0)
, m_BoardHeight(0)
{
	InitGame();
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, MAKEINTRESOURCE(IDC_VALIDPOS)), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	BITMAP bmpInfo;
	m_bmBoard.GetBitmap(&bmpInfo);

	m_BoardWidth = bmpInfo.bmWidth;
	m_BoardHeight = bmpInfo.bmHeight;

	int cx = m_BoardWidth + 6;
	int cy = m_BoardHeight + 52;
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;

	AfxGetMainWnd()->SetWindowPos( NULL, x, y, cx, cy, SWP_NOZORDER );
	
	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dc );
	CRect rc;
	GetClientRect( &rc );
	CBitmap bm;
	bm.CreateCompatibleBitmap( &dc, rc.Width(), rc.Height() );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&bm));

	DrawBoard(&dcMem);
	DrawPieces(&dcMem);

	if (m_lastSelPoint.row != -1)
	{
		DrawSelect(&dcMem, m_lastSelPoint.row, m_lastSelPoint.col);
	}
	if (m_SelPoint.row != -1 )
	{
		DrawSelect(&dcMem, m_SelPoint.row, m_SelPoint.col);
	}

	DrawMove(&dcMem);

	dc.BitBlt( 0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY );
	dcMem.SelectObject( pbmOld );

	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void CChildView::DrawPieces(CDC* pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmPieces));
	
	for( int nRow = 0; nRow < 10; nRow ++ )
		for( int nCol = 0; nCol < 9; nCol ++ )
		{
			if (Board[nRow][nCol] != NONE)
				//	pDC->BitBlt( 20 + 40 * nCol - 19, 20 + 40 * nRow - 19, 38, 38, &dcMem, 
				//				Board[nRow][nCol] * 38, 0, SRCCOPY );
				pDC->TransparentBlt( 20 + 40 * nCol - 19, 20 + 40 * nRow - 19, 38, 38, &dcMem, 
						Board[nRow][nCol] * 38, 0, 38, 38, RGB(255,0,255) );
		}
	dcMem.SelectObject( pbmOld );
}

void CChildView::DrawBoard(CDC* pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmBoard));
	
	pDC->BitBlt( 0, 0, m_BoardWidth, m_BoardHeight, &dcMem, 0, 0, SRCCOPY );
	dcMem.SelectObject( pbmOld );
}

void CChildView::DrawPiece(CDC* pDC, int nRow, int nCol, int piece)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmPieces));
	if( piece != NONE )
		pDC->TransparentBlt( 20 + 40 * nCol - 19, 20 + 40 * nRow - 19, 38, 38, &dcMem, 
					piece * 38, 0, 38, 38, RGB(255,0,255) );
	dcMem.SelectObject( pbmOld );
}

bool CChildView::InBound(int nRow, int nCol)
{
	return (nRow >= 0 && nRow < 10 &&
			nCol >= 0 && nCol < 9);
}

void CChildView::DrawSelect(CDC* pDC, int nRow, int nCol)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmSelect));

	//pDC->BitBlt( 20 + 40 * nCol - 19, 20 + 40 * nRow - 19, 38, 38, &dcMem, 
	//					0, 0, SRCCOPY );
	pDC->TransparentBlt( 20 + 40 * nCol - 19, 20 + 40 * nRow - 19, 38, 38, &dcMem, 
					0, 0, 38, 38, RGB(255,255,255) );
	dcMem.SelectObject( pbmOld );
}


bool CChildView::isValidMove(int nRowSrc, int nColSrc, int nRowDest, int nColDest)
{
	if (nRowDest == nRowSrc && nColDest == nColSrc)
		return false;
	if(isSameSide(nRowSrc, nColSrc, nRowDest, nColDest))
		return false;
	
	switch (Board[nRowSrc][nColSrc])
	{
	case REDCHARIOT:
	case BLACKCHARIOT:
		if (nRowDest != nRowSrc && nColDest != nColSrc)
			return false;
		if (nRowDest == nRowSrc)
		{
			if (nColDest > nColSrc)
			{
				for (int col = nColSrc + 1; col < nColDest; col++)
					if (Board[nRowDest][col] != NONE)
						return false;
			}
			else
			{
				for (int col = nColDest + 1; col < nColSrc; col++)
					if (Board[nRowDest][col] != NONE)
						return false;
			}
		}
		else
		{
			if (nRowDest > nRowSrc)
			{
				for (int row = nRowSrc + 1; row < nRowDest; row++)
					if (Board[row][nColDest] != NONE)
						return false;
			}
			else
			{
				for (int row = nRowDest + 1; row < nRowSrc; row++)
					if (Board[row][nColDest] != NONE)
						return false;
			}
		}
		break;
	case REDHORSE:
	case BLACKHORSE:
		if (!((abs(nRowDest - nRowSrc) == 1 && abs(nColDest - nColSrc) == 2)
			||(abs(nRowDest - nRowSrc) == 2 && abs(nColDest - nColSrc) == 1)))
			return false;
		int row, col;
		if (abs(nRowDest - nRowSrc) == 2)
		{
			row = (nRowSrc + nRowDest) / 2;
			col = nColSrc;
		}
		else
		{
			row = nRowSrc;
			col = (nColSrc + nColDest) / 2;
		}
		if (Board[row][col] != NONE)
			return false;
		break;
	case REDCANNON:
	case BLACKCANNON:
		if (nRowDest != nRowSrc && nColDest != nColSrc)
			return false;
		if (Board[nRowDest][nColDest] == NONE)
		{
			if (nRowDest == nRowSrc)
			{
				if (nColDest > nColSrc)
				{
					for ( int col = nColSrc +1; col < nColDest; col++)
						if (Board[nRowDest][col] != NONE)
							return false;
				}
				else
				{
					for ( int col = nColDest +1; col < nColSrc; col++)
						if (Board[nRowDest][col] != NONE)
							return false;
				}
			}
			else
			{
				if (nRowDest > nRowSrc)
				{
					for ( int row = nRowSrc +1; row < nRowDest; row++)
						if (Board[row][nColDest] != NONE)
							return false;
				}
				else
				{
					for ( int row = nRowDest +1; row < nRowSrc; row++)
						if (Board[row][nColDest] != NONE)
							return false;
				}
			}
		}
		else
		{
			int nLeapPiece = 0;
			if (nRowDest == nRowSrc)
			{
				if (nColDest > nColSrc)
				{
					for ( int col = nColSrc + 1; col < nColDest; col++)
						if (Board[nRowDest][col] != NONE)
							nLeapPiece++;
				}
				else
				{
					for ( int col = nColDest + 1; col < nColSrc; col++)
						if (Board[nRowDest][col] != NONE)
							nLeapPiece++;
				}
			}
			else
			{
				if (nRowDest > nRowSrc)
				{
					for ( int row = nRowSrc + 1; row < nRowDest; row++)
						if (Board[row][nColDest] != NONE)
							nLeapPiece++;
				}
				else
				{
					for ( int row = nRowDest + 1; row < nRowSrc; row++)
						if (Board[row][nColDest] != NONE)
							nLeapPiece++;
				}
			}
			if (nLeapPiece != 1)
				return false;
		}
		break;
	case REDKING:
		if (Board[nRowDest][nColDest] == BLACKKING)
		{
			if ( nColDest != nColSrc)
				return false;
			for ( int row = nRowSrc - 1; row > nRowDest; row--)
				if (Board[row][nColDest] != NONE)
					return false;
		}
		else
		{
			if (!(7 <= nRowDest && nRowDest <= 9
				&& 3 <= nColDest && nColDest <= 5))
				return false;
			if (abs(nRowDest - nRowSrc) + abs(nColDest - nColSrc) > 1)
				return false;
		}
		break;
	case BLACKKING:
		if (Board[nRowDest][nColDest] == REDKING)
		{
			if ( nColDest != nColSrc)
				return false;
			for ( int row = nRowSrc + 1; row < nRowDest; row++)
				if (Board[row][nColDest] != NONE)
					return false;
		}
		else
		{
			if (!(0 <= nRowDest && nRowDest <= 2
				&& 3 <= nColDest && nColDest <= 5))
				return false;
			if (abs(nRowDest - nRowSrc) + abs(nColDest - nColSrc) > 1)
				return false;
		}
		break;
	case REDADVISER:
		if (!(7 <= nRowDest && nRowDest <= 9
			&& 3 <= nColDest && nColDest <= 5))
			return false;
		if (abs(nRowDest - nRowSrc) != 1 || abs(nColDest - nColSrc) != 1)
			return false;
		break;
	case BLACKADVISER:
		if (!(0 <= nRowDest && nRowDest <= 2
			&& 3 <= nColDest && nColDest <= 5))
			return false;
		if (abs(nRowDest - nRowSrc) != 1 || abs(nColDest - nColSrc) != 1)
			return false;
		break;
	case REDELEPHANT:
		if (!(5 <= nRowDest && nRowDest <= 9))
			return false;
		if (abs(nRowDest - nRowSrc) != 2 || abs(nColDest - nColSrc) != 2)
			return false;
		if (Board[(nRowSrc + nRowDest) / 2][(nColSrc + nColDest) / 2] != NONE)
			return false;
		break;
	case BLACKELEPHANT:
		if (!(0 <= nRowDest && nRowDest <= 4))
			return false;
		if (abs(nRowDest - nRowSrc) != 2 || abs(nColDest - nColSrc) != 2)
			return false;
		if (Board[(nRowSrc + nRowDest) / 2][(nColSrc + nColDest) / 2] != NONE)
			return false;
		break;
	case REDPAWN:
		if (nRowDest > nRowSrc)
			return false;
		if (nRowSrc >= 5 && nRowDest == nRowSrc)
			return false;
		if ( nRowSrc - nRowDest + abs(nColDest - nColSrc) > 1)
			return false;
		break;
	case BLACKPAWN:
		if (nRowDest < nRowSrc)
			return false;
		if (nRowSrc <= 4 && nRowDest == nRowSrc)
			return false;
		if ( nRowDest - nRowSrc + abs(nColDest - nColSrc) > 1)
			return false;
		break;
	default:
		return false;
	}
	return true;
}

bool CChildView::isSameSide(int nRowSrc, int nColSrc, int nRowDest, int nColDest)
{
	if ((isRed(Board[nRowSrc][nColSrc])	&& isRed(Board[nRowDest][nColDest]))
		|| (isBlack(Board[nRowSrc][nColSrc]) && isBlack(Board[nRowDest][nColDest])))
		return true;
	else
		return false;
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	int nRow = (point.y - (20 - 19)) / 40;
	int nCol = (point.x - (20 - 19)) / 40;

	if (m_turn == HUMANPLAYER)
	{
		if ( !InBound(nRow, nCol))
		{
			SetCursor(hCurSorInvalidPos);
			return;
		}
		if (m_SelPoint.row == -1)
		{
			if (isPlayerSide(m_turn, Board[nRow][nCol]))
				SetCursor(hCurSorValidPos);
			else
				SetCursor(hCurSorInvalidPos);
		}
		else
		{
			if (isValidMove(m_SelPoint.row, m_SelPoint.col, nRow, nCol) || isPlayerSide(m_turn, Board[nRow][nCol]))
				SetCursor(hCurSorValidPos);
			else
				SetCursor(hCurSorInvalidPos);
		}
	}
	else
	{
		SetCursor(hCurSorInvalidPos);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	memset(ValidMove, 0, sizeof(ValidMove));

	if (m_gameState == GS_GAMEOVER)
		return;
	if (m_turn == HUMANPLAYER)
	{
		int nRow = (point.y - (20 - 19)) / 40;
		int nCol = (point.x - (20 - 19)) / 40;
		if ( !InBound(nRow, nCol) )
			return;

		if (isPlayerSide(m_turn, Board[nRow][nCol]))
		{
			m_SelPoint.row = nRow;
			m_SelPoint.col = nCol;
			Invalidate(false);
			UpdateWindow();
			return;
		}
		if (m_SelPoint.row != -1 && isValidMove(m_SelPoint.row, m_SelPoint.col, nRow, nCol))
		{
			m_lastSelPoint.row = m_SelPoint.row;
			m_lastSelPoint.col = m_SelPoint.col;
			m_SelPoint.row = nRow;
			m_SelPoint.col = nCol;
			
			Board[m_SelPoint.row][m_SelPoint.col] = Board[m_lastSelPoint.row][m_lastSelPoint.col];
			Board[m_lastSelPoint.row][m_lastSelPoint.col] = NONE;

			Invalidate(false);
			UpdateWindow();
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}
void CChildView::InitGame(void)
{
	m_bmBoard.LoadBitmapW(IDB_BOARD);
	m_bmPieces.LoadBitmapW(IDB_PIECES);
	m_bmSelect.LoadBitmapW(IDB_SELECT);	


	// Load a cursor resource.

 	hCurSorValidPos = AfxGetApp()->LoadCursorW(IDC_VALIDPOS);
	hCurSorInvalidPos = AfxGetApp()->LoadCursorW(IDC_INVALIDPOS);

	m_turn = HUMANPLAYER;
	m_lastSelPoint.row = -1;
	m_lastSelPoint.col = -1;
	m_SelPoint.row = -1;
	m_SelPoint.col = -1;

}

bool CChildView::isRed(int piece)
{
	switch (piece)
	{
	case REDCHARIOT:
	case REDHORSE:
	case REDCANNON:
	case REDKING:
	case REDADVISER:
	case REDELEPHANT:
	case REDPAWN:
		return true;
	default:
		return false;
	}
}

bool CChildView::isBlack(int piece)
{
	switch (piece)
	{
	case BLACKCHARIOT:
	case BLACKHORSE:
	case BLACKCANNON:
	case BLACKKING:
	case BLACKADVISER:
	case BLACKELEPHANT:
	case BLACKPAWN:
		return true;
	default:
		return false;
	}
}

bool CChildView::isPlayerSide(int player, int piece)
{
	if (player == HUMANPLAYER)
		if (isRed(piece)) return true;
	return false;
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	memset(ValidMove, 0, sizeof(ValidMove));
	m_lastSelPoint.row = -1;
	m_lastSelPoint.col = -1;
	m_SelPoint.row = -1;
	m_SelPoint.col = -1;

	int nRow = (point.y - (20 - 19)) / 40;
	int nCol = (point.x - (20 - 19)) / 40;
	if ( !InBound(nRow, nCol) )
		return;

	if (isPlayerSide(m_turn, Board[nRow][nCol]))
	{
		switch (Board[nRow][nCol])
		{
		case REDCHARIOT:
		case BLACKCHARIOT:
			GenRMove(nRow, nCol);
			break;
		case REDHORSE:
		case BLACKHORSE:
			GenHMove(nRow, nCol);
			break;
		case REDCANNON:
		case BLACKCANNON:
			GenCMove(nRow, nCol);
			break;
		case REDKING:
		case BLACKKING:
			GenKMove(nRow, nCol);
			break;
		case REDADVISER:
		case BLACKADVISER:
			GenAMove(nRow, nCol);
			break;
		case REDELEPHANT:
		case BLACKELEPHANT:
			GenEMove(nRow, nCol);
			break;
		case REDPAWN:
		case BLACKPAWN:
			GenPMove(nRow, nCol);
			break;
		}
		Invalidate(false);
		UpdateWindow();
	}
	CWnd::OnRButtonDown(nFlags, point);
}
void CChildView::GenRMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i ++)
	{
		int row = nRow;
		int col = nCol;
		int rowa = rowAdjacent[2 * i + 1];
		int cola = colAdjacent[2 * i + 1];
		for (;;)
		{
			row += rowa;
			col += cola;
			if (!InBound(row, col))
				break;
			if (Board[row][col] == NONE)
				AddMove(row, col);
			else
			{
				if ( !isSameSide(nRow, nCol, row, col))
					AddMove(row, col);
				break;
			}
		}
	}
}
void CChildView::GenHMove(int nRow, int nCol)
{
	for (int i = 0; i < 8; i++)
	{
		int nextrow = nRow + rowHorseAdjacent[i];
		int nextcol = nCol + colHorseAdjacent[i];
		if (InBound(nextrow,nextcol) && isValidMove(nRow, nCol, nextrow,nextcol))
			AddMove(nextrow, nextcol);
	}
}

void CChildView::GenCMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i ++)
	{
		int row = nRow;
		int col = nCol;
		int rowa = rowAdjacent[2 * i + 1];
		int cola = colAdjacent[2 * i + 1];
		for (;;)
		{
			row += rowa;
			col += cola;
			if (!InBound(row, col))
				break;
			if (Board[row][col] == NONE)
				AddMove(row, col);
			else
			{
				row += rowa;
				col += cola;
				while (InBound(row, col) && Board[row][col] == NONE)
				{
					row += rowa;
					col += cola;
				}
				if (!InBound(row, col))
					break;
				if (!isSameSide(nRow, nCol, row, col))
					AddMove(row, col);
				break;
			}
		}
	}
}

void CChildView::GenKMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i++)
	{
		int nextrow = nRow + rowAdjacent[2 * i + 1];
		int nextcol = nCol + colAdjacent[2 * i + 1];
		if (InBound(nextrow,nextcol) && isValidMove(nRow, nCol, nextrow,nextcol))
			AddMove(nextrow, nextcol);
	}
}

void CChildView::GenAMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i++)
	{
		int nextrow = nRow + rowAdjacent[2 * i];
		int nextcol = nCol + colAdjacent[2 * i];
		if (InBound(nextrow,nextcol) && isValidMove(nRow, nCol, nextrow,nextcol))
			AddMove(nextrow, nextcol);
	}
}

void CChildView::GenEMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i++)
	{
		int nextrow = nRow + 2 * rowAdjacent[2 * i];
		int nextcol = nCol + 2 * colAdjacent[2 * i];
		if (InBound(nextrow,nextcol) && isValidMove(nRow, nCol, nextrow,nextcol))
			AddMove(nextrow, nextcol);
	}
}

void CChildView::GenPMove(int nRow, int nCol)
{
	for (int i = 0; i < 4; i++)
	{
		int nextrow = nRow + rowAdjacent[2 * i + 1];
		int nextcol = nCol + colAdjacent[2 * i + 1];
		if (InBound(nextrow,nextcol) && isValidMove(nRow, nCol, nextrow,nextcol))
			AddMove(nextrow, nextcol);
	}
}
void CChildView::AddMove(int nRow, int nCol)
{
	ValidMove[nRow][nCol] = 1;
}

void CChildView::DrawMove(CDC* pDC)
{
	for (int r = 0; r < 10; r++)
	{
		for (int c = 0; c < 9; c++)
		{
				if ( ValidMove[r][c] == 1)
				{
					DrawSelect(pDC, r, c);
				}
		}
	}
}

