
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

const CPoint NEXT_ICON_COD(1069,578);
CRect next_icon_rect;

const CPoint QUICK_ICON_COD(1197, 562);
CRect quick_icon_rect;

const CPoint NEXT_P_ICON_COD(1199, 636);
CRect  next_p_icon_rect;

const CPoint GREEN_BUTTON_COD(856, 553);
CRect green_icon_rect;

const CPoint YELLOW_BUTTON_COD(856, 623);
CRect yellow_icon_rect;

const CPoint RED_BUTTON_COD(856, 700);
CRect red_icon_rect;

const CPoint TIME_COD(1092, 712);

const CPoint LAND_COD(690, 72);
const CPoint EMERGENCY_COD(690, 235);
const CPoint OFF_COD(690, 410);
const CPoint LANE_COD[] = { CPoint(0,0), CPoint(1068, 68), CPoint(1068, 203), CPoint(1068, 377) };
const int PLANE_INTERVAL = 10;

int plane_width, plane_height;
CRect land_rect[10], emergency_rect[10], off_rect[10];
CRect lane_rect[5];

const int INFO_H = 40;
const CPoint INFO_COD(550,550);

int tmp_count=0;

// CAirportMasterView

IMPLEMENT_DYNCREATE(CAirportMasterView, CView)

BEGIN_MESSAGE_MAP(CAirportMasterView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
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

int calc_x(int origin_x, int i)
{
	return origin_x - plane_width * (i-1) - PLANE_INTERVAL * i;
}

CAirportMasterView::CAirportMasterView()
{
	// TODO: 在此处添加构造代码
	basic_ui.Load("pic\\basic_ui.png");
	TransparentPNG(&basic_ui);

	next_icon.Load("pic\\next_icon.png");
	TransparentPNG(&next_icon);
	quick_icon.Load("pic\\quick_icon.png");
	TransparentPNG(&quick_icon);
	next_p_icon.Load("pic\\next_p_icon.png");
	TransparentPNG(&next_p_icon);

	green_button.Load("pic\\green_button.png");
	TransparentPNG(&green_button);

	yellow_button.Load("pic\\yellow_button.png");
	TransparentPNG(&yellow_button);

	red_button.Load("pic\\red_button.png");
	TransparentPNG(&red_button);

	int i;
	for(i=1; i<=PLANE_ICON_NUM; i++)
	{
		CString pth;
		pth.Format("pic\\plane%d.png", i);
		plane_icon[i].Load(pth);
		TransparentPNG(&plane_icon[i]);
	}

	plane_width = plane_icon[1].GetWidth();
	plane_height = plane_icon[1].GetHeight();

	//////////////////////////////////////////////////////////////////////////make rects
	
	for(i=1; calc_x(LAND_COD.x, i) >=0; i++)
	{
		int x = calc_x(LAND_COD.x, i);
		land_rect[i].SetRect(x, LAND_COD.y, x + plane_width, LAND_COD.y + plane_height);
	}

	for(i=1; calc_x(EMERGENCY_COD.x, i)>=0; i++)
	{
		int x = calc_x(EMERGENCY_COD.x, i);
		emergency_rect[i].SetRect(x, EMERGENCY_COD.y, x+plane_width, EMERGENCY_COD.y + plane_height);
	}

	for(i=1; calc_x(OFF_COD.x, i) >=0; i++)
	{
		int x = calc_x(OFF_COD.x , i);
		off_rect[i].SetRect(x, OFF_COD.y, x + plane_width, OFF_COD.y + plane_height);
	}

	for(i=1; i<=3; i++)
	{
		lane_rect[i].SetRect(LANE_COD[i].x , LANE_COD[i].y, LANE_COD[i].x + plane_width, LANE_COD[i].y + plane_height);
	}

	next_icon_rect.SetRect(NEXT_ICON_COD.x, NEXT_ICON_COD.y, 
		NEXT_ICON_COD.x + next_icon.GetWidth(), NEXT_ICON_COD.y + next_icon.GetHeight() );

	quick_icon_rect.SetRect(QUICK_ICON_COD.x, QUICK_ICON_COD.y, 
		QUICK_ICON_COD.x + quick_icon.GetWidth(), QUICK_ICON_COD.y + quick_icon.GetHeight() );

	next_p_icon_rect.SetRect(NEXT_P_ICON_COD.x, NEXT_P_ICON_COD.y, 
		NEXT_P_ICON_COD.x + next_p_icon.GetWidth(), 
		NEXT_P_ICON_COD.y + next_p_icon.GetHeight() );

	green_icon_rect.SetRect(GREEN_BUTTON_COD.x, GREEN_BUTTON_COD.y, 
		GREEN_BUTTON_COD.x + green_button.GetWidth(), GREEN_BUTTON_COD.y + green_button.GetHeight() );

	yellow_icon_rect.SetRect(YELLOW_BUTTON_COD.x, YELLOW_BUTTON_COD.y, 
		YELLOW_BUTTON_COD.x + yellow_button.GetWidth(), YELLOW_BUTTON_COD.y + yellow_button.GetHeight() );

	red_icon_rect.SetRect(RED_BUTTON_COD.x, RED_BUTTON_COD.y, 
		RED_BUTTON_COD.x + red_button.GetWidth(), RED_BUTTON_COD.y + red_button.GetHeight() );

	pointing = false;
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
	//创建缓冲DC
	GetClientRect(&m_client);
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(pDC, m_client.Width(),m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);

	///////////////////////////////////////////////////////////////////////draw UI
	m_cacheDC.FillSolidRect(0,0,m_client.Width(), m_client.Height(), RGB(255,255,255));
	basic_ui.Draw(m_cacheDC, 0,0);
	green_button.Draw(m_cacheDC, GREEN_BUTTON_COD);
	yellow_button.Draw(m_cacheDC, YELLOW_BUTTON_COD);
	red_button.Draw(m_cacheDC, RED_BUTTON_COD);
	next_icon.Draw(m_cacheDC, NEXT_ICON_COD);
	quick_icon.Draw(m_cacheDC, QUICK_ICON_COD);
	next_p_icon.Draw(m_cacheDC, NEXT_P_ICON_COD);

	CFont font;
	font.CreatePointFont(120,"微软雅黑", &m_cacheDC);
	m_cacheDC.SelectObject(font);

	CString buffer("现在时刻：");
	CString now_time = pDoc->GetNowTime();
	buffer = buffer + now_time;
	m_cacheDC.TextOut(TIME_COD.x, TIME_COD.y, buffer);

	////////////////////////////////////////////////////////////draw queues

	int now_size = pDoc->GetQSize(LAND);
	int i;
	for(i=1; i<=min(now_size, MAX_QNUM); i++)
	{
		int now_color = pDoc->GetPointPlane(LAND, i).color_num;
		plane_icon[now_color].Draw(m_cacheDC, land_rect[i].TopLeft());
	}

	now_size = pDoc->GetQSize(EMERGENCY);
	for(i=1; i<=min(now_size, MAX_QNUM); i++)
	{
		int now_color = pDoc->GetPointPlane(EMERGENCY, i).color_num;
		plane_icon[now_color].Draw(m_cacheDC, emergency_rect[i].TopLeft());
	}

	now_size = pDoc->GetQSize(OFF);
	for (i=1; i<=min(now_size, MAX_QNUM); i++)
	{
		int now_color = pDoc->GetPointPlane(OFF, i).color_num;
		plane_icon[now_color].Draw(m_cacheDC, off_rect[i].TopLeft());
	}

	////////////////////////////////////////////////////////////draw lanes
	for(i=1; i<=3; i++)
	{
		if(pDoc->LaneEmpty(i) == false)
		{
			int now_color = pDoc->GetLanePlane(i).color_num;
			plane_icon[now_color].Draw(m_cacheDC, lane_rect[i].TopLeft());
		}
	}
	////////////////////////////////////////////////////////////////////////// show info

	CString buf;
	if(pointing)
	{
		CString plane_id = point_plane.id.c_str();
		buf = plane_id + "号航班";
		m_cacheDC.TextOut(INFO_COD.x, INFO_COD.y, buf);

		buf = "进入队列时间：";
		buf += pDoc->TimeToString(point_plane.time);
		m_cacheDC.TextOut(INFO_COD.x, INFO_COD.y + INFO_H, buf);

		buf = "已经等待时间：";
		buf += pDoc->TimeToString(pDoc->now_time - point_plane.time);
		m_cacheDC.TextOut(INFO_COD.x, INFO_COD.y + 2*INFO_H, buf);

		if(point_plane.arrive == true)
		{
			buf.Format( "剩余油量：%d", point_plane.fuel);
			m_cacheDC.TextOut(INFO_COD.x, INFO_COD.y + 3*INFO_H, buf);
		}
	}
	else
	{
		m_cacheDC.FillSolidRect(INFO_COD.x, INFO_COD.y, 200, 200, RGB(255,255,255));
	}

	pDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC,0,0,SRCCOPY);  

	ValidateRect(m_client);
	m_cacheCBitmap.DeleteObject();
	m_cacheDC.DeleteDC();
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

void Refresh()
{

}

void CAirportMasterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAirportMasterDoc *pDoc = GetDocument();

	if( next_icon_rect.PtInRect(point) || quick_icon_rect.PtInRect(point) || next_p_icon_rect.PtInRect(point))
	{
		if(next_icon_rect.PtInRect(point))
		{
			bool ok = pDoc->NextStep();
			if(!ok)
				MessageBox("文件已经处理完毕了……");

			CString output = pDoc->GetOutput();
			p_edit->SetWindowText(output);
			pDoc->ClearOutput();

		}
		
		else if(quick_icon_rect.PtInRect(point))
		{
			int i;
			for(i=1; i<=QUICK_STEPS; i++)
			{
				bool ok = pDoc->NextStep();
				if(!ok)
				{
					MessageBox("文件已经处理完毕了……");
					break;
				}
			}
			CString output = pDoc->GetOutput();
			p_edit->SetWindowText(output);
			pDoc->ClearOutput();
		}

		else if(next_p_icon_rect.PtInRect(point))
		{
			int target_time = pDoc->next_plane.time;
			while(pDoc->now_time < target_time)
			{
				bool ok = pDoc->NextStep(); // 这个函数会加载新飞机从而修改next_plane.time
				if(!ok)
				{
					MessageBox("文件已经处理完毕了……");
					break;
				}
			}
			CString output = pDoc->GetOutput();
			p_edit->SetWindowText(output);
			pDoc->ClearOutput();
		}

		CDC* pdc = GetDC();
		OnDraw(pdc);
		pdc->DeleteDC();
	}
	else if( green_icon_rect.PtInRect(point) || yellow_icon_rect.PtInRect(point) || red_icon_rect.PtInRect(point))
	{
		tmp_count++;
		char buf[60]={};
		sprintf_s(buf, "TMP_%03d", tmp_count);
		string id(buf);

		if(green_icon_rect.PtInRect(point))
		{
			CAirplane tmp(id, false, pDoc->now_time, 0 );
			pDoc->NewOff(tmp);
			pDoc->Refresh(OFF);
		}
		else if(yellow_icon_rect.PtInRect(point))
		{
			CAirplane tmp(id, true, pDoc->now_time, rand()%(FUEL_MAX - FUEL_MIN) + FUEL_MIN );
			pDoc->NewLand(tmp);
			pDoc->Refresh(LAND);
		}
		else if(red_icon_rect.PtInRect(point))
		{
			CAirplane tmp(id, true, pDoc->now_time, rand()%(FUEL_MIN - 1) + 1  );
			pDoc->NewEmergency(tmp);
			pDoc->Refresh(EMERGENCY);
		}

		CString output = pDoc->GetOutput();
		p_edit->SetWindowText(output);

		CDC* pdc = GetDC();
		OnDraw(pdc);
		pdc->DeleteDC();
	}

	CView::OnLButtonDown(nFlags, point);
	return;
}


void CAirportMasterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	p_edit = new CEdit();
	p_edit->Create(WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_READONLY | 
		ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN, 
		CRect(12, 541, 524, 771), this, WM_USER+100);
	p_edit->ShowWindow(SW_SHOW);
	
	font_edit.CreateFont(22, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		DEFAULT_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("微软雅黑")); // lpszFac 

	p_edit->SetFont(&font_edit);
}


void CAirportMasterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pointing = false;
	CAirportMasterDoc *pDoc = GetDocument();

	//////////////////////////////////////////////////////////////////////////queues
	int i;
	int now_size = pDoc->GetQSize(LAND);
	for(i=1; i<=now_size;i++)
	{
		if( land_rect[i].PtInRect(point) )
		{
			pointing = true;
			point_plane = pDoc->GetPointPlane(LAND, i);
			break;
		}
	}

	if(!pointing)
	{
		int now_size = pDoc->GetQSize(EMERGENCY);
		for(i=1; i<=now_size;i++)
		{
			if( emergency_rect[i].PtInRect(point) )
			{
				pointing = true;
				point_plane = pDoc->GetPointPlane(EMERGENCY, i);
				break;
			}
		}
	}

	if(!pointing)
	{
		int now_size = pDoc->GetQSize(OFF);
		for(i=1; i<=now_size;i++)
		{
			if( off_rect[i].PtInRect(point) )
			{
				pointing = true;
				point_plane = pDoc->GetPointPlane(OFF, i);
				break;
			}
		}
	}

	if(!pointing)
	{
		for(i=1; i<=3; i++)
		{
			if(lane_rect[i].PtInRect(point) && !pDoc->LaneEmpty(i))
			{
				pointing = true;
				point_plane = pDoc->GetLanePlane(i);
			}
		}
	}

	if(pointing)
	{
		CDC *pDC = GetDC();
		OnDraw(pDC);
		pDC->DeleteDC();
	}
	CView::OnMouseMove(nFlags, point);
}
