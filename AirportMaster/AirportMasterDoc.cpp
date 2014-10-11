
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

void CAirportMasterDoc::AppendMessage(string plane_name, int lane_num, int event_type)
{
	char tmp[50];
	const char *plane_name_c = plane_name.c_str();
	if(event_type == OFF_LANE)
	{
		sprintf_s(tmp, "%s�ź����뿪���ܵ���", plane_name_c);
	}
	else if(event_type == TURN_EMERGENCY)
	{
		sprintf_s(tmp, "%s�ź���ȼ�Ͳ��㣬׼���������䣡", plane_name_c);
	}
	else if(event_type == NEW_LAND)
	{
		sprintf_s(tmp, "%s�ź����ڿ����������ȴ����䡣", plane_name_c);
	}
	else if(event_type == NEW_OFF)
	{
		sprintf_s(tmp, "%s�ź���������׼������ʼ�ȴ���ɡ�", plane_name_c);
	}
	else if(event_type == ASSIGN)
	{
		sprintf_s(tmp, "%s�ź����׼����%d���ܵ���", plane_name_c, lane_num);
	}
	else if(event_type == GO_AWAY)
	{
		sprintf_s(tmp, "����ǰ������ɻ����࣬%s�ź����ѱ�ת�����ڽ��������䡣", plane_name_c);
	}
	output_buffer += tmp + CString("\r\n");
}

void CAirportMasterDoc::ArrangeLanes()
{
	//Emergency
	int i=3;
	while(emergency_q.empty() == false)
	{
		CAirplane now_plane = emergency_q.front();
		while(i>=1)
		{
			if(lane[i].empty())
			{
				emergency_q.pop();
				lane[i].assign(now_plane, now_time + LANE_TIME);
				AppendMessage(now_plane.id, i, ASSIGN);
				break;
			}
			else
				i--;
		}
		if(i==0)
			break;

	}

	//take off special
	if(lane[3].empty() && !take_off_q.empty() )
	{
		CAirplane plane_off = take_off_q.front();
		take_off_q.pop();
		lane[3].assign(plane_off, now_time + LANE_TIME);
		AppendMessage(plane_off.id, 3, ASSIGN);
	}

	i=2;
	while( i>=1 && !(take_off_q.empty() && land_q.empty() ) )
	{
		while(!lane[i].empty() && i>=1)
			i--;
		if(i<1)
			break;

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
			AppendMessage(plane_go.id, i, ASSIGN);
			take_off_q.pop();
			i--;
		}
		else
		{
			lane[i] .assign( plane_land, now_time + LANE_TIME);
			AppendMessage(plane_land.id, i, ASSIGN);
			land_q.pop();
			i--;
		}
	}
	return;
}

int CAirportMasterDoc::StringToTime(string input)
{
	int hour = 10*(input[0]-'0') + (input[1] - '0');
	int min = 10*(input[3]-'0') + (input[4]- '0');

	return hour * 60 + min;
}

bool CAirportMasterDoc::NeedTransport(int fuel, int size)
{
	int t = fuel/DFUEL;
	if(size / 3 < t)
		return false;
	return true;
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
		target.fuel = rand() % (20-10) + 10; 
	}
	else
	{
		target.arrive = false;
		target.time = StringToTime(go_time);
	}

	if(arrive_time.length() >= 5) //"00:00"
		target.time = StringToTime(arrive_time);

	target.color_num = CAirplane::now_color;
	CAirplane::now_color = CAirplane::now_color % PLANE_ICON_NUM + 1;
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
			lane[i].clear();
			AppendMessage(plane_name, i, OFF_LANE);
		}
	}

	//drop fuel
	CAirplane list[50];
	int p_list = 0;
	land_q.scan(ALERT, list, p_list);
	for(i=1; i<=p_list; i++)
	{
		if( NeedTransport(list[i].fuel, emergency_q.size()) )
		{
			AppendMessage(list[i].id, 0, GO_AWAY);
		}
		else
		{
			emergency_q.push(list[i]);
			AppendMessage(list[i].id, 0, TURN_EMERGENCY);
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
			AppendMessage(next_plane.id, 0, NEW_LAND);
		}
		else
		{
			take_off_q.push(next_plane);
			AppendMessage(next_plane.id, 0, NEW_OFF);
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

	land_q.Fill(plane_in_q[LAND]);
	emergency_q.Fill(plane_in_q[EMERGENCY]);
	take_off_q.Fill(plane_in_q[OFF]);

	return true;
}

CString CAirportMasterDoc::TimeToString(int t)
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

void CAirportMasterDoc::ClearOutput()
{
	output_buffer.Empty();
}

CAirplane CAirportMasterDoc::GetPointPlane(int type, int num)
{
	return plane_in_q[type][num];
}

CAirplane CAirportMasterDoc::GetLanePlane(int num)
{
	return lane[num].now_plane;
}