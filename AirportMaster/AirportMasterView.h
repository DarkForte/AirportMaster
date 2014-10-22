
// AirportMasterView.h : CAirportMasterView 类的接口
//

#pragma once

class CAirportMasterView : public CView
{
protected: // 仅从序列化创建
	CAirportMasterView();
	DECLARE_DYNCREATE(CAirportMasterView)

// 特性
public:
	CAirportMasterDoc* GetDocument() const;

//变量
	CImage next_icon, quick_icon, next_p_icon;
	CImage green_button, yellow_button, red_button;
	CImage basic_ui;
	CImage plane_icon[10];

	CRect m_client;
	CDC m_cacheDC;   //缓冲DC  
	CBitmap m_cacheCBitmap;//缓冲位图  

	CEdit *p_edit;

	CFont font_edit;

	bool pointing;
	CAirplane point_plane;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CAirportMasterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // AirportMasterView.cpp 中的调试版本
inline CAirportMasterDoc* CAirportMasterView::GetDocument() const
   { return reinterpret_cast<CAirportMasterDoc*>(m_pDocument); }
#endif

