unit InterfaceDisplay;

interface

uses
    WKSStructs, InterfaceObj, InterfaceDisplayTransformation, InterfaceGeometry,
    Windows;

type TSymbolType = Longint;
const SYMBOLTYPE_POINT =    TSymbolType(1);
const SYMBOLTYPE_LINE =     TSymbolType(2);
const SYMBOLTYPE_FILL =     TSymbolType(3);
const SYMBOLTYPE_TEXT =     TSymbolType(4);

type TDrawResult = Longint;
const LAYERDRAW_NORMAL =            TDrawResult(0);
const LAYERDRAW_NOVISIBLE =         TDrawResult(1);
const LAYERDRAW_TRACKCANCEL =       TDrawResult(2);
const LAYERDRAW_EXCEEDLIMIT =       TDrawResult(3);
const LAYERDRAW_NOREADY =           TDrawResult(4);
const LAYERDRAW_DISPLAYNOREADY =    TDrawResult(5);
const LAYERDRAW_NOCACHE =           TDrawResult(6);
const LAYERDRAW_NOSUPPORT =         TDrawResult(7);
const LAYERDRAW_UNEXPECTED =        TDrawResult(8);

type
ISymbol = interface;
IDisplay = interface(IPersist)
    //����任
    function SetDisplayTransformation(const pTrans: IDisplayTransformation): Boolean; stdcall;
    function GetDisplayTransformation(out ppTrans: IDisplayTransformation): Boolean; stdcall;

    //DC
    function SetDC(const dc: HDC): Boolean; stdcall;
    function GetDC(out dc: HDC): Boolean; stdcall;

    //�����С
    function SetRect(var rect: TRect): Boolean; stdcall;
    function GetRect(out rect: TRect): Boolean; stdcall;

    //����ɫ
    function SetBackgroundColor(const color: COLORREF): Boolean; stdcall;
    function GetBackgroundColor(): COLORREF; stdcall;

    //  StartSraw��FinishDraw����ͳһ����
    //  ĳһ��Display�ϵ����л��Ʋ���
    function StartDraw(): Boolean; stdcall;
    function FinishDraw(): Boolean; stdcall;
    function IsDrawing(): Boolean; stdcall;

    //  ���ڻ��Ƶĵ㡢�ߡ������
    function SetSymbol(const pSymbol: ISymbol): Boolean; stdcall;
    function GetSymbol(const symboltype: TSymbolType; out ppSymbol: ISymbol): Boolean; stdcall;

    //  ���Ƽ��ζ��󣬿��Ի���Geometry����Ҳ����
    //  ֱ�ӻ���Stream���ڼӿ��ٶȲ�������
    function DrawGeometry(const pGeometry: IGeometry): Boolean; stdcall;
    function DrawStream(pStream: IStreamX): Longword; stdcall;

    //  ���Ʊ�ע���ı���
    function DrawText(const pGeometry: IGeometry; const pcText: PChar;
        out textenvelope: TRect): Boolean; stdcall;
    function DrawTextXY(const x: Double; const y: Double; const pcText: PChar;
        out textenvelope: TRect): Boolean; stdcall;

    procedure EraseContent(const pEnvelope: PWKSRect); stdcall;

    //��Display�еķɻ�����destdc�ϣ������Ե��������豸���ڳߴ�
    function RefreshWindow(const destdc: HDC; const destrect: TRect; rop: Longword): Boolean; stdcall;

    //
    function RefreshWindow1(): Boolean; stdcall;
end;

//================================================================================
//  ����һ���ڴ�DC����������֧��ѡ�񼯵Ļ��Ƶ�
//================================================================================
IDisplayCache = interface(IPersist)
    //����һ���ڴ�dc��ָ��͸��������
    function CreateCache(const bgcolor: COLORREF; const alpha: Byte;
        const transparent: Boolean; const rop: Longword): Longint; stdcall;

    //���е��ڴ�dc����
    function GetCacheCount(): Longint; stdcall;

    //ȡ����index��dc��id��Ȼ����Ը������idɾ����GetCacheDC
    function GetCacheID(const index: Longint; out cacheid: Longint): Boolean; stdcall;

    //�ɵ�
    function DeleteCache(const cacheid: Longint): Boolean; stdcall;
    procedure ClearAllCaches(); stdcall;

    //����cache size
    function SetLockCacheSize(const cacheid: Longint; const lock: Boolean): Boolean; stdcall;
    function GetLockCacheSize(const cacheid: Longint; out lock: Boolean): Boolean; stdcall;
    function SetCacheSize(const cacheid: Longint; const width: Longword;
        const height: Longword): Boolean; stdcall;

    //�ҵ�id��dc
    function GetCacheDC(const cacheid: Longint; out dc: HDC): Boolean; stdcall;

    //���ڴ�dc���Ƶ������dc��
    function PostCache(const dc: HDC; const cacheid: Longint): Boolean; stdcall;
    function PostCache1(const dc: HDC; const cacheid: Longint; const delta_x: Longint;
        const delta_y: Longint): Boolean; stdcall;
    function PostCache2(const dc: HDC; const cacheid: Longint; var partial: TRect): Boolean; stdcall;

    //���ڴ�dc���Ƶ�primary buffer��
    function PostCacheToPrimary(const cacheid: Longint): Boolean; stdcall;
    function PostCacheToPrimary1(const cacheid: Longint; const delta_x: Longint;
        const delta_y: Longint): Boolean; stdcall;
    function PostCacheToPrimary2(const cacheid: Longint; var partial: TRect): Boolean; stdcall;

    //�����ڴ�dc�ı���ɫ��symbol
    function SetCacheBGColor(const cacheid: Longint; const color: COLORREF): Boolean; stdcall;
    function GetCacheBGColor(const cacheid: Longint; out color: COLORREF): Boolean; stdcall;
    function SetCacheSymbol(const cacheid: Longint; const pSymbol: ISymbol): Boolean; stdcall;
    function GetCacheSymbol(const cacheid: Longint; const symboltype: TSymbolType;
        out ppSymbol: ISymbol): Boolean; stdcall;

    //����geometry���ڴ�dc��
    function DrawCacheGeometry(const cacheid: Longint; const pGeometry: IGeometry): Boolean; stdcall;
    function DrawCacheStream(const cacheid: Longint; pStream: IStreamX): Longword; stdcall;
    function DrawCacheText(const cacheid: Longint; const pGeometry: IGeometry;
        const pcText: PChar; out textenvelope: TRect): Boolean; stdcall;
    function DrawCacheTextXY(const cacheid: Longint; const x: double; const y: double;
        const pcText: PChar; out textenvelope: TRect): Boolean; stdcall;

    //��primary buffer���Ƶ��ڴ�dc��
    function CopyPrimaryToCache(const cacheid: Longint; const delta_x: Longint;
        const delta_y: Longint): Boolean; stdcall;
    function CopyPrimaryToCache1(const cacheid: Longint; var partial: TRect): Boolean; stdcall;

    //��������һ��
    function CopyCacheToCache(const fromid: Longint; const toid: Longint;
        const delta_x: Longint; const delta_y: Longint): Boolean; stdcall;
    function CopyCacheToCache1(const fromid: Longint; const toid: Longint;
        var partial: TRect): Boolean; stdcall;

    //���ڴ�dc���Ϊ����ɫ
    function EraseCacheContent(const cacheid: Longint; const pEnvelope: PWKSRect): Boolean; stdcall;
    procedure EraseCachesContent(const pEnvelope: PWKSRect); stdcall;

    //��ָ����cache����window dc��
    function PostCacheToWindow(const cacheid: Longint): Boolean; stdcall;
end;


IScreenBrowser = interface(IPersist)
    //  ��Ļ���Σ�����
    function PanStart(var screenpoint: tagPOINT): Boolean; stdcall;
    function PanMoveTo(var screenpoint: tagPOINT): Boolean; stdcall;
    function PanStop(): Boolean; stdcall;
    function Paning(): Boolean; stdcall;

    //  �ı���ʾ�����ߣ��������ķ���
    function MapScaleAt(const mapscale: Double): Boolean; stdcall;

    //  ������Ļ���ĵ�
    function MapCenterAt(var wndpnt: tagPOINT): Boolean; stdcall;
    function MapCenterAt1(var center: WKSPoint): Boolean; stdcall;

    //  ���η���
    function VisibleExtentIn(var rect: TRect): Boolean; stdcall;
    function VisibleExtentOut(var rect: TRect): Boolean; stdcall;
    function VisibleMapExtentIn(var extent: WKSRect): Boolean; stdcall;
    function VisibleMapExtentOut(var extent: WKSRect): Boolean; stdcall;
end;


ISymbol = interface(IPersist)
    function SetCode(var code: Longint): Boolean; stdcall;
    function GetCode(): Longint; stdcall;
    function SetName(const name: PChar): Boolean; stdcall;
    function GetName(): PChar; stdcall;
    function GetDC(out dc: HDC): Boolean; stdcall;
    function GetDisplayTransformation(out ppTrans: IDisplayTransformation): Boolean; stdcall;
    function GetROP2(out rop2: Longint): Boolean; stdcall;

    function Prepare(const dc: HDC; const pTrans: IDisplayTransformation;
        const rop2: Longint): Boolean; stdcall;
    function Draw(const pGeometry: IGeometry): Boolean; stdcall;
    function DrawStream(const pStream: IStreamX): Longword; stdcall;
    function SetColor(const color: COLORREF): Boolean; stdcall;
    function GetColor(out color: COLORREF): Boolean; stdcall;
    procedure SetColorLock(const lock: Boolean); stdcall;
    procedure GetColorLock(out lock: Boolean); stdcall;
    function GetSymbolType(): TSymbolType; stdcall;
end;

IPointSymbol = interface(ISymbol)
    function SetAngle(const angle: Double): Boolean; stdcall;
    function GetAngle(out angle: Double): Boolean; stdcall;
    function SetOffset(const x: Double; const y: Double): Boolean; stdcall;
    function GetOffset(out x: Double; out y: Double): Boolean; stdcall;
    function _ParentOffset(var offset: WKSPoint; const angle: Double;
        const size: Double): Boolean; stdcall;
end;

ILineSymbol = interface(ISymbol)
    function SetOffset(const offset: Double): Boolean; stdcall;
    function GetOffset(out offset: Double): Boolean; stdcall;
    function _ParentOffset(const offset: Double): Boolean; stdcall;
end;

IFillSymbol = interface(ISymbol)
end;

ITextSymbol = interface(ISymbol)
    function Draw(const pGeometry: IGeometry; out textenvelope: TRect): Boolean; stdcall;
    function SetFont(var font: LOGFONT): Boolean; stdcall;
    function GetFont(out font: LOGFONT): Boolean; stdcall;
    function SetWidth(const width: Double): Boolean; stdcall;
    function GetWidth(out width: Double): Boolean; stdcall;
    function SetHeight(const height: Double): Boolean; stdcall;
    function GetHeight(out height: Double): Boolean; stdcall;
    function SetAngle(const angle: Double): Boolean; stdcall;
    function GetAngle(out angle: Double): Boolean; stdcall;
    function SetText(const text: PChar): Boolean; stdcall;
    function GetText(): PChar; stdcall;
end;


implementation

end.
