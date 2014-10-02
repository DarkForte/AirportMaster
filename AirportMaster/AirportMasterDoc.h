
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
	CAirplane plane_in_q[5][10];

	CString output_buffer;
// 操作
public:
	void ArrangeLanes();
	bool ReadNext(CAirplane &target);
	bool NextStep();

	CString GetNowTime();
	CQueue GetQueue(int type);
	int GetQSize(int type);
	bool LaneEmpty(int num);

	static const int OFF_LANE = 1, TURN_EMERGENCY =2, NEW_LAND=3, NEW_OFF=4, ASSIGN=5;
	void AppendMessage(string plane_name, int lane_num, int event_type);
	CString GetOutput();
	void ClearOutput();

	CAirplane GetPointPlane(int type, int num);
	CAirplane GetLanePlane(int num);

	static int StringToTime(string input);
	static CString TimeToString(int t);

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
