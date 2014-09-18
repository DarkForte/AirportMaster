
// AirportMasterDoc.h : CAirportMasterDoc ��Ľӿ�
//


#pragma once
#include "Queue.h"
#include "Lane.h"
#include <fstream>
using namespace std;

class CAirportMasterDoc : public CDocument
{
protected: // �������л�����
	CAirportMasterDoc();
	DECLARE_DYNCREATE(CAirportMasterDoc)

// ����
public:
	CQueue take_off_q, land_q, emergency_q;
	CLane lane[4];
	ifstream fin;
	int now_time;
	CAirplane next_plane;
// ����
public:
	void ArrangeLanes();
	bool ReadNext(CAirplane &target);
	bool NextStep();

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CAirportMasterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
