
// AirportMasterView.h : CAirportMasterView ��Ľӿ�
//

#pragma once


class CAirportMasterView : public CView
{
protected: // �������л�����
	CAirportMasterView();
	DECLARE_DYNCREATE(CAirportMasterView)

// ����
public:
	CAirportMasterDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CAirportMasterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AirportMasterView.cpp �еĵ��԰汾
inline CAirportMasterDoc* CAirportMasterView::GetDocument() const
   { return reinterpret_cast<CAirportMasterDoc*>(m_pDocument); }
#endif

