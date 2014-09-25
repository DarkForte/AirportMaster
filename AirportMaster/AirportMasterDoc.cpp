
// AirportMasterDoc.cpp : CAirportMasterDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "AirportMaster.h"
#endif

#include "AirportMasterDoc.h"
#include "Queue.h"
#include "Lane.h"

#include <propkey.h>
#include <fstream>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAirportMasterDoc

IMPLEMENT_DYNCREATE(CAirportMasterDoc, CDocument)

BEGIN_MESSAGE_MAP(CAirportMasterDoc, CDocument)
END_MESSAGE_MAP()


// CAirportMasterDoc ����/����

CAirportMasterDoc::CAirportMasterDoc()
{
	// TODO: �ڴ����һ���Թ������
	srand(unsigned(time(0)));

	fin.open("input\\air_input.txt");
	ReadNext(next_plane);
	now_time = next_plane.time - 1;
}

CAirportMasterDoc::~CAirportMasterDoc()
{
}

BOOL CAirportMasterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CAirportMasterDoc ���л�

void CAirportMasterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CAirportMasterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CAirportMasterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CAirportMasterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAirportMasterDoc ���

#ifdef _DEBUG
void CAirportMasterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAirportMasterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CAirportMasterDoc ����
void CAirportMasterDoc::ArrangeLanes()
{
	//Emergency
	int i;
	while(emergency_q.empty() == false)
	{
		CAirplane now_plane = emergency_q.front();
		emergency_q.pop();

		for(i=3;i>=1;i--)
		{
			if(lane[i].empty())
			{
				lane[i].assign(now_plane, now_time + LANE_TIME);
				break;
			}
		}

	}

	//take off special
	if(lane[3].empty() && !take_off_q.empty() )
	{
		CAirplane plane_off = take_off_q.front();
		take_off_q.pop();
		lane[3].assign(plane_off, now_time + LANE_TIME);
	}

	i=2;
	while( i>=1 && !(take_off_q.empty() && land_q.empty() ) )
	{
		CAirplane plane_land;
		CAirplane plane_go;
		if(land_q.empty() == false)
			 plane_land = land_q.front();
		else
			plane_land.time = INF;

		if(take_off_q.empty() == false)
			plane_go = take_off_q.front();
		else
			plane_go.time = INF;

		if(plane_go.time < plane_land.time)
		{
			lane[i] .assign( plane_go, now_time + LANE_TIME);
			take_off_q.pop();
			i--;
		}
		else
		{
			lane[i] .assign( plane_land, now_time + LANE_TIME);
			land_q.pop();
			i--;
		}
	}
	return;
}

int StringToTime(string input)
{
	int hour = 10*(input[0]-'0') + (input[1] - '0');
	int min = 10*(input[3]-'0') + (input[4]- '0');

	return hour * 60 + min;
}

bool CAirportMasterDoc::ReadNext(CAirplane &target)
{
	string plane_name, go_time, arrive_time;
	if(fin>>plane_name>>go_time>>arrive_time == false)
	{
		return false;
	}
	
	plane_name[plane_name.length()-1] = '\0';
	target.id = plane_name;

	if(go_time.length() < 5)//"00:00,"
	{
		target.arrive = true;
		target.fuel = rand() % (100-10) + 10; 
	}
	else
	{
		target.arrive = false;
		target.time = StringToTime(go_time);
	}

	if(arrive_time.length() >= 5) //"00:00"
		target.time = StringToTime(arrive_time);

	return true;
}

bool CAirportMasterDoc::NextStep()
{
	now_time++;
	//clear lanes
	int i;
	for(i=1; i<=3; i++)
	{
		if(now_time == lane[i].leave_time)
		{
			string plane_name = lane[i].now_plane.id;
			output_buffer.AppendFormat("%s�ź����뿪���ܵ�\n", plane_name);
			lane[i].clear();
		}
	}

	//arrange lanes
	ArrangeLanes();

	//process new plane
	while(next_plane.time == now_time)
	{
		if(next_plane.arrive == true)
		{
			land_q.push(next_plane);
		}
		else
		{
			take_off_q.push(next_plane);
		}

		bool ok = ReadNext(next_plane);
		if(!ok)
		{// still have to wait until all lane empty
			bool all_lane_empty = true;
			for(i=1; i<=3; i++)
			{
				if(lane[i].empty() == false)
				{
					all_lane_empty = false;
					break;
				}
			}
			if(all_lane_empty)
				return false;
		}
	}

	return true;
}

CString TimeToString(int t)
{
	CString ret;
	int hour = t / 60;
	int min = t%60;

	ret.Format("%02d:%02d", hour, min);
	return ret;
}

CString CAirportMasterDoc::GetNowTime()
{
	return TimeToString(now_time);
}

CQueue CAirportMasterDoc::GetQueue(int type)
{
	if(type == LAND)
		return land_q;
	else if(type == EMERGENCY)
		return emergency_q;
	else
		return take_off_q;
	
}

int CAirportMasterDoc::GetQSize(int type)
{
	if(type == LAND)
		return land_q.size();
	else if(type == EMERGENCY)
		return emergency_q.size();
	else
		return take_off_q.size();
}

bool CAirportMasterDoc::LaneEmpty(int num)
{
	return lane[num].empty();
}

CString CAirportMasterDoc::GetOutput()
{
	return output_buffer;
}