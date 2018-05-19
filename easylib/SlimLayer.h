#if !defined(SLIMLAYER_INCLUDED_)
#define SLIMLAYER_INCLUDED_

#include "CustomEditLayer.h"
#include "CellQuadTree.h"
#include "Fields.h"
#include "NetTopoBase.h"

using namespace easymap::net;

namespace easymap
{

class CVectorLayer;
typedef TSmartPtr<CVectorLayer> CVectorLayerPtr;

class CSlimLayer;
typedef TSmartPtr<CSlimLayer> CSlimLayerPtr;

class CSlimFeature;
typedef TSmartPtr<CSlimFeature> CSlimFeaturePtr;

typedef long SlimEditType;
const SlimEditType SLIMEDIT_NEW     = 1;
const SlimEditType SLIMEDIT_MODIFY  = 2;
const SlimEditType SLIMEDIT_DELETE  = 3;

const long SLIM_RESERVED_LENGTH = 250;

//================================================================================
//  ʸ��Layer��֧�ּ��ζ������ӡ�ɾ�����޸ġ�ѡ�񡢲�ѯ
//  �������Ϊ��һ��㶨SlimLayer��ShapeLayer
//================================================================================
class CVectorLayer : public CCustomEditLayer
{
CLASS_NAME(CVectorLayer)    //ע������������д_GetClassName()������

protected:
    CVectorLayer();

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp)
    {
        if (_invalid(pp)) return false;
        assert(!*pp);

        if ((0 == strcmp(interfacename, "IObj"))
            || (0 == strcmp(interfacename, "IPersist"))
            || (0 == strcmp(interfacename, "CPersist"))
            || (0 == strcmp(interfacename, "ILayer")))
        {
            *pp = static_cast<ILayer*>(this);
        }
        else if (0 == strcmp(interfacename, "IEditLayer"))
        {
            *pp = static_cast<IEditLayer*>(this);
        }
        else if ((0 == strcmp(interfacename, "CCustomEditLayer"))
            || (0 == strcmp(interfacename, "CVectorLayer")))
        {
            *pp = this;
        }
        else
        {
            return this->_GotoInterface(interfacename, pp);//��������֧�־��������
        }

        static_cast<IObj*>(*pp)->_AddRef();
        return true;
    };

private:
    //��������֧�־��������
    virtual bool _GotoInterface(const char* const interfacename, void** pp) = 0;

public:
    virtual void GetPrecision(double& precision) const = 0;
    virtual void SetRefScale(const double& scale) = 0;
    virtual void GetRefScale(double& scale) const = 0;

    virtual dword Select(const vector<dword>& fids, const bool append = true) = 0;
    virtual dword Deselect(const vector<dword>& fids) = 0;
    virtual dword GetSelection(vector<dword>& fids) const = 0;

    virtual bool GetFids(vector<dword>& fids) const = 0;
    virtual dword GetFeatureCount() const = 0;
    virtual dword AddFeature(const IGeometryPtr pGeometry, const string& fieldvalues,
        const string& annotation) = 0;
    virtual bool GetFeature(const dword fid, IGeometryPtr& pGeometry,
        string& fieldvalues, string& annotation) = 0;
    virtual bool GetFeatureGeometry(const dword fid, IGeometryPtr& pGeometry) const = 0;
    virtual bool GetFeatureMBR(const dword fid, WKSRect& mbr) const = 0;
    virtual bool DeleteFeature(const dword fid) = 0;
    virtual bool CreateFeature(IVectorFeaturePtr& pFeature) = 0;
    virtual bool GetFeature(const dword fid, IVectorFeaturePtr& pFeature) = 0;

    virtual bool RapidModifyPoint(const dword fid, const WKSPoint& point) = 0;

    virtual bool Identify(vector<dword>& fids, const WKSRect& envelope,
        const bool partialselect = true) = 0;

    virtual bool ImpreciseSearch(const WKSRect& extent, vector<dword>& fids) = 0;

    virtual bool SetFields(const string& fields) = 0;
    virtual void GetFields(string& fields) const = 0;

    virtual void GetGeometryColumnInfo(GeometryColumnInfo& geocolinfo) const = 0;

    virtual void GetFields(CFieldsPtr& pFields) const = 0;
    virtual bool SetDisplayField(const long fieldindex) = 0;
    virtual long GetDisplayField() const = 0;

    virtual bool SetDefaultSymbol(const ISymbolPtr pSymbol, const bool save2esd) = 0;
    virtual bool GetDefaultSymbol(ISymbolPtr& pSymbol) const = 0;

    virtual void SetRendererType(const SlimRendererType renderertype, const bool save2esd) = 0;
    virtual SlimRendererType GetRendererType() const = 0;
    virtual bool SetSymbol(const string& key, const ISymbolPtr pSymbol) = 0;
    virtual bool GetSymbol(const string& key, ISymbolPtr& pSymbol) const = 0;
    virtual bool GetSymbolByIndex(const dword index, string& key, ISymbolPtr& pSymbol) const = 0;
    virtual dword GetSymbolCount() const = 0;
    virtual void ClearSymbols() = 0;
    virtual bool SetRendererField(const long fieldindex, const bool save2esd) = 0;
    virtual long GetRendererField() const = 0;
    virtual void SetShowDefaultSymbol(const bool showdefaultsymbol, const bool save2esd) = 0;
    virtual bool GetShowDefaultSymbol() const = 0;

    virtual bool ReadOnly() const = 0;

    //----------------------------------------------------------------------------
    //  �������
    //----------------------------------------------------------------------------
    //  �����ݲ��������·���㡢�ϰ����
    virtual void SetNetTolerance(const double tolerance) = 0;
    virtual double GetNetTolerance() const = 0;

    //  �����������磬field < 0������geometry�ĳ��ȴ���ͼ
    virtual bool CreateNetTopo(const long field, const bool bidirectional) = 0;
    virtual bool CreateNetTopo2(const dword field_from_to, const dword field_to_from) = 0;

    //  ·����
    virtual bool AddNetRoute(const WKSPoint& route) = 0;
    virtual bool RemoveNetRoute(const WKSPoint& route) = 0;
    virtual bool GetNetRoutes(IMultiPoint** ppRoutes) const = 0;
    virtual void ClearNetRoutes() = 0;

    //  �ϰ���
    virtual bool AddNetBarrierPoint(const WKSPoint& barrier) = 0;
    virtual bool RemoveNetBarrierPoint(const WKSPoint& barrier) = 0;
    virtual bool GetNetBarrierPoints(IMultiPoint** ppBarriers) const = 0;
    virtual void ClearNetBarrierPoints() = 0;

    //  �ϰ���
    virtual bool AddNetBlockedBiEdge(const dword fid) = 0;
    virtual bool AddNetBlockedSingleEdge(const WKSPoint& from, const WKSPoint& to) = 0;
    virtual bool RemoveNetBlockedBiEdge(const dword fid) = 0;
    virtual bool RemoveNetBlockedSingleEdge(const WKSPoint& from, const WKSPoint& to) = 0;
    virtual dword GetNetBlockedEdgeCount() const = 0;
    virtual bool GetNetBlockedEdgeByIndex(const dword i, dword& fid, WKSPoint& from, WKSPoint& to) const = 0;
    virtual bool GetNetBlockedEdgeIDs(vector<dword>& fids) const = 0;
    virtual void ClearNetBlockedEdges() = 0;

    //  �������·��
    virtual bool DoBestPath(IPath** ppPath, IIntArray** ppFids) = 0;

    //  �����������
    virtual void ClearNetTopo() = 0;

    //  �洢�������絽�����ļ���
    virtual bool StoreNetTopo() = 0;
    virtual bool RestoreNetTopo() = 0;
    //----------------------------------------------------------------------------
};


//================================================================================
//  SlimData ʸ��Layer���ȿ�ȫ�������ڴ��У�Ҳ�ɷ����ļ���
//  ע�ⴴ��ʱΪ���ڴ��У�ִ����AttachToFile()��ͷ����ļ�����
//================================================================================
class CSlimLayer : public CVectorLayer
{
//���������ֹ�CLASS_NAME(CSlimLayer)
friend class _ClassFactory_CSlimLayer;
PERSIST_DUMP(CSlimLayer)
EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    //--------- SlimLayer�������� ---------
    struct SlimHeader
    {
        char            Reserved[SLIM_RESERVED_LENGTH];
        char            Alias[120];
        char            MainVersion;
        char            SubVersion;
        dword           IndexBlockSize;
        char            FieldsInfo[2000];
        long            DisplayField;
    };

private:
    struct Feature
    {
        dword           fid;
        WKSRect         mbr;
        IGeometryPtr    geometry;
        string          fieldvalues;
        string          annotation;
    };

    //�༭��ԭ�Ӳ���
    struct EditAtom
    {
        SlimEditType edittype;
        dword fid;
        dword olddelta;
        dword newdelta;
        WKSRect oldmbr;
        WKSRect newmbr;
    };

    //ÿ�α༭undo�����ݣ���һ������ԭ�Ӳ������
    struct EditProcess
    {
        vector<EditAtom> atoms;
    };


private:
    CSlimLayer();
    CSlimLayer(const CSlimLayer& slimlayer);
public:
    //����һ��SlimLayer�������ڴ���
    CSlimLayer(
        const ShapeType     shapetype,
        const MapUnits      mapunit,
        const double&       basescale,
        const double&       precision,
        const WKSRect&      extent,
        const long          indexlevel,
        const CFieldsPtr    pFields,
        const bool          annotation
        );

    //��һ���Ѿ����ڵ��ļ�
    //  filemap = true  -> ʹ���ڴ�ӳ���ļ������٣������ڴ�
    //  filemap = false -> ʹ������ļ���ȡ��ʽ����С�ڴ�ռ�ã�֧�ִ��ļ�
    CSlimLayer(const string& filename, const bool readonly = false,
        const bool filemap = true);
private:
    ~CSlimLayer();

    //׼���õȴ��������¼�����
    void InitEventTypes();

    //��ʼ��һЩ�ɻ�
    void InitialSlimLayer(CStreamPtr pStream);

    //���ļ��󶨵�this
    void FromFile(const string& filename, const bool readonly,
        const bool filemap);

    //...
    bool CheckHeader() const;

    //�㲻����
    void GotoHell();

private:
    SlimHeader              m_Header;
    GeometryColumnInfo      m_GeoColumnInfo;
    WKSRect                 m_DataExtent;
    dword                   m_MaxFID;
    dword                   m_FeatureCount;

    double                  m_ReferenceScale;
    CStreamPtr              m_pFeatureData;
    CStreamPtr              m_pIndexStream;
    map<dword, dword>       m_FidIndex;         //fid������fid -> delta
    CMultiQuadTreePtr       m_pSpatialIndex;
    list<dword>             m_SelectedFIDs;
    ISymbolPtr              m_pDefaultSymbol;
    map<string, ISymbolPtr> m_SymbolMap;
    long                    m_RendererField; //-1: fid  ��0��ʼΪ��һ�������ֶ�
    SlimRendererType        m_RendererType;
    bool                    m_ShowDefaultSymbol;
    IPointPtr               m_pAuxPoint;    //���ڼ���RapidModifyPoint()

    vector<EditProcess>     m_UndoProcs;        //��¼ÿһ�α༭������ÿһ�ζ��ɶ��ԭ�Ӳ�����ɣ�
    EditProcess             m_EditProc;         //����һ��SetUndoPoint֮������иĶ�
                                                //���Ϊempty��û���κ��޸�

    long                    m_UndoIndex;        //��¼��ǰ��Ҫundo������һ��process
    bool                    m_SaveDirty;

    bool                    m_ModifyDirty;      //�Ƿ�ִ�й���ӡ�ɾ�����޸�Ҫ�صĲ�������savedata��
                                                //��������������Ƿ�Index2DataStream

    FILETIME                m_LastTime;

    NetTopo                 m_Net;
    bool                    m_FastShortest;

    string        m_SR;

private:
    void SelectObjectsByEnvelope(vector<dword>& resultfids, const WKSRect& envelope,
        const bool partialselect);
    void Feature2Stream(const Feature& feature);   //��featureд��m_pFeatureData��ǰλ��
    void Stream2Feature(Feature& feature);         //��m_pFeatureData��ǰλ���ж�ȡfeature

    void Index2Stream();
    void Stream2Index();

    void Index2DataStream();                    //������д���������ļ�
    void DataStream2Index();                    //���������ļ��ж�������

    void Symbols2Stream();
    void Stream2Symbols();

    void UpdateFixedHeader();
    void ReloadFixedHeader();

    bool EditUndoRestoreIndex(const EditProcess& ep);   //��ep�����ݻָ�����
    bool EditRedoRestoreIndex(const EditProcess& ep);   //��ep�����ݻָ�����

    void CleanEditUndo();                       //�ӵ�û��setundopoint������

    void WriteModifyTime();
    bool ModifiedByOther();
    void RereadIndexStream();

private:
    dword PresaveInstance(CStreamPtr pStream, void* const assist) const;
    dword PreloadInstance(CStreamPtr pStream, void* const assist);

    DrawResult DrawLayerData(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancelPtr   pTrackCancel    = NULL
        );

    DrawResult DrawLayerSelection(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancelPtr   pTrackCancel    = NULL
        );

    bool GetSlimData(CStreamPtr pStream);

public:
    const char* const __stdcall _GetClassName() const//!
    {
        return "CSlimLayer";
    };

    bool _GotoInterface(const char* const interfacename, void** pp);

    bool __stdcall GetExtent(WKSRect& fullext) const;
    MapUnits __stdcall GetMapUnit() const;
    bool __stdcall GetBaseScale(double& scale) const;
    const char* __stdcall GetSpatialReference() const;
    void GetPrecision(double& precision) const;
    void SetRefScale(const double& scale);
    void GetRefScale(double& scale) const;

    bool __stdcall SetUndoPoint();
    bool __stdcall EditUndoable() const;
    bool __stdcall EditRedoable() const;
    bool __stdcall EditUndo();
    bool __stdcall EditRedo();
    bool __stdcall EditCancel();
    bool __stdcall SaveData();
    bool __stdcall IsDirty() const;

    bool SetDefaultSymbol(const ISymbolPtr pSymbol, const bool save2esd);
    bool GetDefaultSymbol(ISymbolPtr& pSymbol) const;

    //����֧���ʵ�����
    void SetRendererType(const SlimRendererType renderertype, const bool save2esd);
    SlimRendererType GetRendererType() const;
    bool SetSymbol(const string& key, const ISymbolPtr pSymbol);
    bool GetSymbol(const string& key, ISymbolPtr& pSymbol) const;
    bool GetSymbolByIndex(const dword index, string& key, ISymbolPtr& pSymbol) const;
    dword GetSymbolCount() const;
    void ClearSymbols();
    bool SetRendererField(const long fieldindex, const bool save2esd);
    long GetRendererField() const;
    void SetShowDefaultSymbol(const bool showdefaultsymbol, const bool save2esd);
    bool GetShowDefaultSymbol() const;

    bool GetFids(vector<dword>& fids) const;
    dword GetFeatureCount() const;
    dword AddFeature(const IGeometryPtr pGeometry, const string& fieldvalues,
        const string& annotation);
    bool SetFeature(const dword fid, const IGeometryPtr pGeometry,
        const string& fieldvalues, const string& annotation);
    bool GetFeature(const dword fid, IGeometryPtr& pGeometry,
        string& fieldvalues, string& annotation);
    bool GetFeatureGeometry(const dword fid, IGeometryPtr& pGeometry) const;
    bool GetFeatureMBR(const dword fid, WKSRect& mbr) const;
    bool DeleteFeature(const dword fid);

    bool CreateFeature(IVectorFeaturePtr& pFeature);
    bool GetFeature(const dword fid, IVectorFeaturePtr& pFeature);

    bool RapidModifyPoint(const dword fid, const WKSPoint& point);

    dword Select(const WKSPoint& point, const bool append = true);
    dword __stdcall Select(const WKSRect& envelope, const bool partialselect = true,
        const bool append = true);
    dword Select(const vector<dword>& fids, const bool append = true);
    dword Deselect(const WKSPoint& point);
    dword __stdcall Deselect(const WKSRect& envelope, const bool partialselect = true);
    dword Deselect(const vector<dword>& fids);
    dword GetSelection(vector<dword>& fids) const;
    dword __stdcall GetSelectCount() const;
    void __stdcall ClearSelection();

    bool Identify(vector<dword>& fids, const WKSRect& envelope,
        const bool partialselect = true);

    bool ImpreciseSearch(const WKSRect& extent, vector<dword>& fids);

    bool SetFields(const string& fields);
    void GetFields(string& fields) const;

    void GetFields(CFieldsPtr& pFields) const;
    bool SetDisplayField(const long fieldindex);
    long GetDisplayField() const;

    bool SetAlias(const string& alias);
    string GetAlias() const;

    void GetGeometryColumnInfo(GeometryColumnInfo& geocolinfo) const;
    bool SetSR(const string& sr);

    //�󶨵�ָ�����ļ���
    //  filemap = true  -> ʹ���ڴ�ӳ���ļ������٣������ڴ�
    //  filemap = false -> ʹ������ļ���ȡ��ʽ����С�ڴ�ռ�ã�֧�ִ��ļ�
    bool AttachToFile(const string& filename, const bool filemap = true);

    //���ذ󶨵��ڴ�ӳ���ļ�����������ڴ��оͷ���""
    string GetFileName() const;

    bool ReadOnly() const;

    void SetNetTolerance(const double tolerance);
    double GetNetTolerance() const;
    bool CreateNetTopo(const long field, const bool bidirectional);
    bool CreateNetTopo2(const dword field_from_to, const dword field_to_from);
    bool AddNetRoute(const WKSPoint& route);
    bool RemoveNetRoute(const WKSPoint& route);
    bool GetNetRoutes(IMultiPoint** ppRoutes) const;
    void ClearNetRoutes();
    bool AddNetBarrierPoint(const WKSPoint& barrier);
    bool RemoveNetBarrierPoint(const WKSPoint& barrier);
    bool GetNetBarrierPoints(IMultiPoint** ppBarriers) const;
    void ClearNetBarrierPoints();
    bool AddNetBlockedBiEdge(const dword fid);
    bool AddNetBlockedSingleEdge(const WKSPoint& from, const WKSPoint& to);
    bool RemoveNetBlockedBiEdge(const dword fid);
    bool RemoveNetBlockedSingleEdge(const WKSPoint& from, const WKSPoint& to);
    dword GetNetBlockedEdgeCount() const;
    bool GetNetBlockedEdgeByIndex(const dword i, dword& fid, WKSPoint& from, WKSPoint& to) const;
    bool GetNetBlockedEdgeIDs(vector<dword>& fids) const;
    void ClearNetBlockedEdges();
    bool DoBestPath(IPath** ppPath, IIntArray** ppFids);
    void ClearNetTopo();
    bool StoreNetTopo();
    bool RestoreNetTopo();

    bool Valid() const;

friend class CShapeLayer;
};
//================================================================================
CLASS_FACTORY(CSlimLayer)

//================================================================================
//  ���ڸ�������slimlayer�е�Ҫ��
//================================================================================
class CSlimFeature : public IVectorFeature
{
CLASS_NAME(CSlimFeature)
EVENTS_DISPATCHER
NO_EVENTS_LISTENER

private:
    CSlimFeature();
    ~CSlimFeature();

private:
    dword           m_fid;
    IGeometryPtr    m_geometry;
    CFieldValuesPtr m_pFieldValues;
    string          m_Annotation;
    CSlimLayerPtr   m_slimlayer;

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;

    dword __stdcall GetFID() const;
    bool __stdcall GetMBR(WKSRect& mbr) const;
    void GetLayer(CVectorLayerPtr& pLayer);
    bool SetGeometryRef(const IGeometryPtr pGeometry);
    void GetGeometryRef(IGeometryPtr &pGeometry);
    bool SetGeometry(const IGeometryPtr pGeometry);
    void GetGeometry(IGeometryPtr &pGeometry);
    void SetFieldValues(const string& fieldvalues);
    string FieldValuesAsString() const;
    bool __stdcall Delete();
    bool __stdcall Update();

    void __stdcall GetLayer(ILayer** ppLayer);
    bool __stdcall SetGeometryRef(const IGeometry* const pGeometry);
    void __stdcall GetGeometryRef(IGeometry** ppGeometry);
    dword __stdcall GetFieldCount() const;
    bool __stdcall SetFieldValue(const dword index, const char* const fieldvalue);
    bool __stdcall GetFieldValue(const dword index, IFieldValue** ppFieldValue) const;
    void __stdcall SetAnnotation(const char* const annotation);
    const char* __stdcall GetAnnotation() const;

friend class CSlimLayer;
};
//================================================================================

bool CheckTrackCancel(const ITrackCancelPtr pTrackCancel, const long cacheid);

}

#endif
