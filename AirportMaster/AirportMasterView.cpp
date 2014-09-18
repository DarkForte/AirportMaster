
// AirportMasterView.cpp : CAirportMasterView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "AirportMaster.h"
#endif

#include "AirportMasterDoc.h"
#include "AirportMasterView.h"
#include <stdlib.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CPoint NEXT_ICON_COD(1111,557);
const CPoint GREEN_BUTTON_COD(856, 553);
const CPoint YELLOW_BUTTON_COD(856, 623);
const CPoint RED_BUTTON_COD(856, 700);

// CAirportMasterView

IMPLEMENT_DYNCREATE(CAirportMasterView, CView)

BEGIN_MESSAGE_MAP(CAirportMasterView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CAirportMasterView 构造/析构

CAirportMasterView::CAirportMasterView()
{
	// TODO: 在此处添加构造代码
	basic_ui.Load(L"pic\\basic_ui.png");
	next_icon.Load(L"pic\\next_icon.png");
	
	green_button.Load(L"pic\\green_button.png");
	yellow_button.Load(L"pic\\yellow_button.png");
	red_button.Load(L"pic\\red_button.png");
	plane_icon.Load(L"pic\\plane.png");

}

CAirportMasterView::~CAirportMasterView()
{
}

BOOL CAirportMasterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAirportMasterView 绘制

void CAirportMasterView::OnDraw(CDC* pDC)
{
	CAirportMasterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	//draw UI
	basic_ui.Draw(pDC->m_hDC, 0,0);
	green_button.Draw(pDC->m_hDC, GREEN_BUTTON_COD);
	yellow_button.Draw(pDC->m_hDC, YELLOW_BUTTON_COD);
	red_button.Draw(pDC->m_hDC, RED_BUTTON_COD);
	next_icon.Draw(pDC->m_hDC, NEXT_ICON_COD);
}

void CAirportMasterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAirportMasterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAirportMasterView 诊断

#ifdef _DEBUG
void CAirportMasterView::AssertValid() const
{
	CView::AssertValid();
}

void CAirportMasterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAirportMasterDoc* CAirportMasterView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAirportMasterDoc)));
	return (CAirportMasterDoc*)m_pDocument;
}
#endif //_DEBUG


// CAirportMasterView 消息处理程序


void CAirportMasterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAirportMasterDoc *pDoc = GetDocument();

	if(NEXT_ICON_COD.x <= point.x && point.x<= NEXT_ICON_COD.x + next_icon.GetWidth() 
		&& NEXT_ICON_COD.y <= point.y && point.y <= NEXT_ICON_COD.y + next_icon.GetHeight() )
	{
		bool ok = pDoc->NextStep();
		if(!ok)
			MessageBox(L"文件已经处理完毕了……");
	}

	CView::OnLButtonDown(nFlags, point);
	return;
}
