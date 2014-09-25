
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
#include "Queue.h"
#include "QNode.h"
#include <stdlib.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CPoint NEXT_ICON_COD(1111,557);
const CPoint GREEN_BUTTON_COD(856, 553);
const CPoint YELLOW_BUTTON_COD(856, 623);
const CPoint RED_BUTTON_COD(856, 700);
const CPoint TIME_COD(1092, 712);
const CPoint LAND_COD(690, 72);
const CPoint EMERGENCY_COD(690, 235);
const CPoint OFF_COD(690, 410);
const CPoint LANE_COD[] = { CPoint(0,0), CPoint(1068, 68), CPoint(1068, 203), CPoint(1068, 377) };
const int PLANE_INTERVAL = 10;

int plane_width, plane_height;

// CAirportMasterView

IMPLEMENT_DYNCREATE(CAirportMasterView, CView)

BEGIN_MESSAGE_MAP(CAirportMasterView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CAirportMasterView 构造/析构

void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] *pucColor[3] / 255;
			pucColor[1] = pucColor[1] *pucColor[3] / 255;
			pucColor[2] = pucColor[2] *pucColor[3] / 255;
		}
	}
}

CAirportMasterView::CAirportMasterView()
{
	// TODO: 在此处添加构造代码
	basic_ui.Load("pic\\basic_ui.png");
	next_icon.Load("pic\\next_icon.png");
	
	green_button.Load("pic\\green_button.png");
	yellow_button.Load("pic\\yellow_button.png");
	red_button.Load("pic\\red_button.png");
	plane_icon.Load("pic\\plane.png");

	TransparentPNG(&plane_icon);
	plane_width = plane_icon.GetWidth();
	plane_height = plane_icon.GetHeight();

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

int calc_x(int origin_x, int i)
{
	return origin_x - plane_width * (i-1) - PLANE_INTERVAL * i;
}

void CAirportMasterView::OnDraw(CDC* pDC)
{
	CAirportMasterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	///////////////////////////////////////////////////////////////////////draw UI
	basic_ui.Draw(pDC->m_hDC, 0,0);
	green_button.Draw(pDC->m_hDC, GREEN_BUTTON_COD);
	yellow_button.Draw(pDC->m_hDC, YELLOW_BUTTON_COD);
	red_button.Draw(pDC->m_hDC, RED_BUTTON_COD);
	next_icon.Draw(pDC->m_hDC, NEXT_ICON_COD);

	CFont font;
	font.CreatePointFont(120,"微软雅黑", pDC);
	pDC->SelectObject(font);

	CString buffer("现在时刻：");
	CString now_time = pDoc->GetNowTime();
	buffer = buffer + now_time;
	pDC->TextOut(TIME_COD.x, TIME_COD.y, buffer);

	////////////////////////////////////////////////////////////draw queues

	int now_size = pDoc->GetQSize(LAND);
	int i;
	for(i=1; i<=now_size && calc_x(LAND_COD.x, i) >=0; i++)
	{
		plane_icon.Draw(pDC->m_hDC, calc_x(LAND_COD.x, i), LAND_COD.y);
	}

	now_size = pDoc->GetQSize(EMERGENCY);
	for(i=1; i<=now_size && calc_x(EMERGENCY_COD.x, i)>=0; i++)
	{
		plane_icon.Draw(pDC->m_hDC, calc_x(EMERGENCY_COD.x, i), EMERGENCY_COD.y);
	}

	now_size = pDoc->GetQSize(OFF);
	for (i=1; i<=now_size && calc_x(OFF_COD.x, i) >=0; i++)
	{
		plane_icon.Draw(pDC->m_hDC, calc_x(OFF_COD.x, i) , OFF_COD.y);
	}

	////////////////////////////////////////////////////////////draw lanes
	for(i=1; i<=3; i++)
	{
		if(pDoc->LaneEmpty(i) == false)
		{
			plane_icon.Draw(pDC->m_hDC, LANE_COD[i]);
		}
	}
	
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
			MessageBox("文件已经处理完毕了……");
		else
		{
			CString output = pDoc->GetOutput();
			p_edit->SetWindowText(output);

			CDC* pdc = GetDC();
			OnDraw(pdc);
			pdc->DeleteDC();
		}
	}

	

	CView::OnLButtonDown(nFlags, point);
	return;
}


void CAirportMasterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	p_edit = new CEdit();
	p_edit->Create(WS_VISIBLE| ES_LEFT | ES_READONLY, CRect(12, 541, 524, 771), this, WM_USER+100);
	p_edit->ShowWindow(SW_SHOW);
	CFont font;
	font.CreateFont(22, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		TRUE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		DEFAULT_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Arial")); // lpszFac 

	p_edit->SetFont(&font);
}
