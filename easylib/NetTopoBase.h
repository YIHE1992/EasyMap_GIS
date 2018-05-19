#if !defined(NETTOPOBASE_INCLUDED_)
#define NETTOPOBASE_INCLUDED_

//�������˽ṹ���㷨

#pragma warning(disable: 4786)
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

#include "Stream.h"
#include "..\\include\\WKSInclude.h"

namespace easymap
{

namespace net
{

//-------------------------------------------------
//ע��������еĶ���id��0������Ч
//-------------------------------------------------

//����
struct Vertex : public WKSPoint
{
    inline bool operator==(const Vertex& rhs) const
    {
        return (this->vid == rhs.vid) ? true : false;
    };
    inline bool operator>(const Vertex& rhs) const
    {
        return (this->vid > rhs.vid) ? true : false;
    };
    inline bool operator<(const Vertex& rhs) const
    {
        return (this->vid < rhs.vid) ? true : false;
    };

    inline Vertex()
    {
        vid = 0;
    };

    inline Vertex(const Vertex& v)
    {
        x = v.x;
        y = v.y;
        vid = v.vid;
    };

    inline Vertex& operator=(const Vertex& rhs)
    {
        if (this == &rhs) return *this;
        x = rhs.x;
        y = rhs.y;
        vid = rhs.vid;
        return *this;
    };

    dword vid;
};


//�ߣ�ע���ǵ����
struct Edge
{
    Edge()
        {from = to = 0; eid = 0; weight = 0;};
    dword from;                 //���vertex id
    dword to;                   //�յ�vertex id
    double weight;              //��Ȩ
    long eid;                  //��id���㷨��û�ã�����������
};


//ת��㣬����2��
struct TurnPoint
{
    inline TurnPoint()
    {
        turnpoint.vid = from.vid = to.vid = 0;
        blocked = 0;
        weight = 0;
        tid = 0;
    };
    Vertex turnpoint;           //ת���
    Vertex from;                //��һ�������
    Vertex to;                  //�ڶ������յ�
    long blocked;               //�ڸ÷����ϣ�1 - ������0 - ͨ��
    double weight;              //ת���Ȩֵ
    dword tid;                  //ת���id
};

//�ڲ�ʹ�ã�����ĳһ���㵽v��Ȩֵ���Լ���id������VertexNeighbors��
struct Vertex_to
{
    inline Vertex_to()
        {v.vid = 0; to_weight = 0; eid = 0;};
    inline Vertex_to(const Vertex& vertex, const double w, const long edge_id)
        {v = vertex; to_weight = w; eid = edge_id;};
    Vertex v;
    double to_weight;   //���������VertexNeighbors�б��涥�㵽neighbor���Ȩֵ
    long eid;  //�Ӷ��㵽v�ıߵ�eid���㷨��û�ã�����������
};

//�ڲ�ʹ�ã���������ڵ㣨with weight���б�
struct VertexNeighbors
{
    VertexNeighbors()
        {vid = 0;};
    dword vid;                              //����id
    map<dword, Vertex_to> vertexes;         //���ڶ����б�����ÿ�����ڶ��㵽�ö����Ȩֵ��keyΪvid
};

//�ڲ�ʹ�ã�Ȩֵ��id��֧��weight�ıȽ�����
struct WeightID
{
    inline WeightID()
    {
        weight = 0; vid = 0; eid = 0;
    };

    inline WeightID(const double w, const dword w_vid, const dword w_eid)
    {
        weight = w; vid = w_vid; eid = w_eid;
    };

    inline bool operator==(const WeightID& rhs) const
    {
        return (this->weight == rhs.weight) ? true : false;
    };

    inline bool operator>(const WeightID& rhs) const
    {
        return (this->weight > rhs.weight) ? true : false;
    };

    inline bool operator<(const WeightID& rhs) const
    {
        return (this->weight < rhs.weight) ? true : false;
    };

    double weight;
    dword vid;
    long eid;
};


//=======================================================================
//  ����·�������˽ṹ���Լ�һ���򻯵����·���㷨
//=======================================================================
class NetTopo
{
private:
    double _tolerance;                      //�ݲ�

    //-------------------------------------------------------------------
    //  �����б�
    //-------------------------------------------------------------------
    dword _max_vid;
    map<dword, Vertex> _vertexes;           //�����б�
                                            //keyΪ����vid

    dword _max_tid;
    map<dword, TurnPoint> _turnpoints;      //ת����б�
                                            //keyΪת���tid

    //-------------------------------------------------------------------
    //  ����ľ�̬���˽ṹ
    //-------------------------------------------------------------------
    map<dword, VertexNeighbors*> _topo_map; //���� <==> ���ڵ�ӳ��
                                            //keyΪ�ö���vid
                                            //valueΪ�Ըö���Ϊ���ĵ�����յ�
    //-------------------------------------------------------------------


    //-------------------------------------------------------------------
    //  ˳��·���㡢�ϰ��㼰�ϰ��ߣ�keyΪ_vertexes�е�key
    //-------------------------------------------------------------------
    vector<dword>   _routes;
    set<dword>      _barriers;
    vector<Edge>    _blockedge;
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //  �������·��ʱ����ʱ�ṹ��keyΪ����vid
    //-------------------------------------------------------------------
    set<dword> _vs_untag;                   //δ��ǽڵ��б�
    set<dword> _vs_temptag;                 //��ʱ��ǽڵ��б�
    set<dword> _vs_permtag;                 //���ñ�ǽڵ��б�
    //-------------------------------------------------------------------


    //  ͨ�������Ҷ���
    dword _findvertex(const WKSPoint& point) const;

    //  ������·���㼰�ϰ���֮ǰ�ж��Ƿ��ظ�
    bool _checkrepeat(const dword vid) const;

    //  ����֮ǰԤ����̬�ṹ
    void _before();

    //  ���������ָ�ԭʼ��̬�ṹ
    void _after();


    //-------------------------------------------------------------------
    //  �򻯵Ŀ������·���㷨������Ȩ���ٶȱȽϿ죬�����ܺ���
    //-------------------------------------------------------------------
    //  ��������·�������㷨
    //  pathΪ·���Ķ���key�б�ע��from����ӵ�path��
    //  ����ֵΪfalse������ʧ�ܣ�û�ҵ��յ㣩
    bool _fastshortest_2vertexes(const dword from, const dword to, vector<dword>& path,
        vector<long>& edge_ids);

    //  �ݹ麯����lastΪ�Ѿ��ҵ�����һ�����㣬toΪ�յ�
    //  ����true�����ҵ��յ㣬�����ɹ�
    bool _fastshortest_step(const dword last, const Vertex& to, vector<dword>& path,
        vector<long>& edge_ids);

    //  ˳�򾭹������������·��������pathΪ·����˳�򶥵��б�edge_idsΪ�����ı�id�б�
    //  ����ֵΪfalse������ʧ��
    bool _fastshortest(vector<dword>& path, vector<long>& edge_ids);
    //-------------------------------------------------------------------

public:
    NetTopo();
    ~NetTopo();

    void SetTolerance(const double tolerance);
    double GetTolerance() const;

    //  ������еľ�̬�ṹ����ʱ�ṹ��·���㡢�ϰ��㡢ת���
    void ClearAll();

    bool equal(const double& a, const double& b) const;
    bool equal(const WKSPoint& a, const WKSPoint& b) const;

    //  ������������һ����
    //  ע��edge_idΪ�������Զ���ı�id���㷨�Դ˲���Ҫ��
    void add_edge(const WKSPoint& from, const WKSPoint& to, const double& weight,
        const long edge_id, bool bidirectional = true);

    //  ���ӡ�ɾ��·����
    bool add_route(const WKSPoint& route);
    Vertex get_route(const WKSPoint& route) const;
    Vertex get_route(const dword i) const;
    bool remove_route(const WKSPoint& route);
    dword get_routecount() const;
    void clear_routes();

    //  ���ӡ�ɾ���ϰ���
    bool add_barrier_vertex(const WKSPoint& barrier);
    Vertex get_barrier_vertex(const WKSPoint& barrier) const;
    Vertex get_barrier_vertex(const dword i) const;
    bool remove_barrier_vertex(const WKSPoint& barrier);
    dword get_barrier_vertexcount() const;
    void clear_barrier_vertexes();

    //  ���ӡ�ɾ���ϰ���
    //  ע������ı�edge_idΪ�û����壬�㷨����Ҫ��
    //  һ��edge_id���Զ�Ӧ��������
    bool add_blocked_edge(const WKSPoint& from, const WKSPoint& to,
        Edge& blocked_edge);
    bool get_blocked_edge(const WKSPoint& from, const WKSPoint& to,
        Edge& blocked_edge) const;
    bool remove_blocked_edge(const WKSPoint& from, const WKSPoint& to);
    dword add_blocked_edges(const long blocked_edgeid);
    dword remove_blocked_edges(const long blocked_edgeid);
    dword add_blocked_lineedges(const WKSPoint& point_on_edge, vector<Edge>& blocked_edges);
    dword get_blocked_lineedges(const WKSPoint& point_on_edge, vector<Edge>& blocked_edges) const;
    dword get_blocked_edgecount() const;
    bool get_blocked_edge_byindex(const dword i, long& blocked_edgeid,
        WKSPoint& from, WKSPoint& to) const;
    void clear_blocked_edges();


    //  �򻯵Ŀ������·���㷨������ֵ��true - �ɹ��ҵ��յ�
    //  �����vs��·��˳�򶥵��б�edge_idsΪ·����id�б�
    bool fastshortest(vector<Vertex>& vs, vector<long>& edge_ids);

friend dword NetTopo2Stream(const NetTopo& nettopo, CStreamPtr pStream);
friend dword Stream2NetTopo(CStreamPtr pStream, NetTopo& nettopo);
};


//  nettopo <-> stream
//  ������ľ�̬�ṹ��·���㼰�ϰ������л�
dword NetTopo2Stream(const NetTopo& nettopo, CStreamPtr pStream);
dword Stream2NetTopo(CStreamPtr pStream, NetTopo& nettopo);

}

}

#endif
