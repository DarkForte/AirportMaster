
// AirportMasterDoc.h : CAirportMasterDoc 类的接口
//


#pragma once
#include "Queue.h"
#include "Lane.h"
#include <fstream>
using namespace std;

class CAirportMasterDoc : public CDocument
{
protected: // 仅从序列化创建
	CAirportMasterDoc();
	DECLARE_DYNCREATE(CAirportMasterDoc)

// 特性
public:
	CQueue take_off_q, land_q, emergency_q;
	CLane lane[4];
	ifstream fin;
	int now_time;
	CAirplane next_plane;
// 操作
public:
	void ArrangeLanes();
	bool ReadNext(CAirplane &target);
	bool NextStep();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CAirportMasterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
