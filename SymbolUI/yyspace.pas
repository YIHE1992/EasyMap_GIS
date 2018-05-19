unit yyspace;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls, Forms, Graphics;


//�������ж�����Ƶ�polyline��polygon�������������Ƶ�ʱ�����ֻ������ô��ĵ�
const MAXPOINTSINPOLYLINE = 30000;
const MAXPOINTSINPOLYGON = MAXPOINTSINPOLYLINE;

//���ڽ����Ƶ�<����/�����>���ظ�������
type
    TyyShapeTracked = record
        m_Points: array[0..MAXPOINTSINPOLYLINE] of tagPOINT;
        m_PointCount: Integer;
    end;

//���ڽ����Ƶ�<����>���ظ�������
type
    TyyRectTracked = record
        m_Point1: tagPOINT;
        m_Point2: tagPOINT;
    end;

//���ڽ����Ƶ�<Բ>���ظ�������
type
    TyyCircleTracked = record
        m_Center: tagPOINT;
        m_Radius: Integer;
    end;



//------------------------------------------------------------------------------
//�¼���Ӧ������ʽ������������紫��Ļص�������������ʽ��
//------------------------------------------------------------------------------
type
    //
    TEVENTPaintHandle = procedure(dc: HDC) of Object;

    //�������С�����ı�ʱ�����������µĸ߶ȺͿ��
    TEVENTSizeHandle = procedure(width, height: Integer) of Object;

    //���������Ļλ�÷����ı�ʱ
    TEVENTMoveHandle = procedure() of Object;

    //����갴������ʱ
    TEVENTMouseDownHandle = procedure(Button: TMouseButton;
                                      Shift: TShiftState;
                                      X: Integer;
                                      Y: Integer
                                      ) of Object;

    //����갴���ɿ�ʱ
    TEVENTMouseUpHandle = procedure(Button: TMouseButton;
                                    Shift: TShiftState;
                                    X: Integer;
                                    Y: Integer
                                    ) of Object;

    //��������ƶ�ʱ
    TEVENTMouseMoveHandle = procedure(Shift: TShiftState;
                                      X: Integer;
                                      Y: Integer
                                      ) of Object;

    //����갴��˫��ʱ
    TEVENTMouseDoubleClickHandle = procedure(Button: TMouseButton;
                                             Shift: TShiftState;
                                             X: Integer;
                                             Y: Integer
                                             ) of Object;

    //�����̰�������ʱ
    TEVENTKeyDownHandle = procedure(Key: Word;
                                    Shift: TShiftState
                                    ) of Object;

    //�����̰����ɿ�ʱ
    TEVENTKeyUpHandle = procedure(Key: Word;
                                  Shift: TShiftState
                                  ) of Object;

    //-------------------------------------------------------------
    //���ڻ���<����>��ÿ����һ���������һ�ε��ã�
    //
    //������
    //      polyline            <����>      �������ص�ǰ�����Ƶ�<����>
    //      ContinueTracking    <���>      �Ƿ�������ƣ�����False�ʹ�����ƽ�����
    //
    //-------------------------------------------------------------
    TEVENTPolylineTrackingHandle = procedure(var Polyline: TyyShapeTracked;
                                             var ContinueTracking: Boolean
                                             ) of Object;

    //-------------------------------------------------------------
    //<����>�������
    //
    //������
    //      polyline            <����>      �������������Ƶ�<����>
    //
    //-------------------------------------------------------------
    TEVENTPolylineTrackedHandle = procedure(var Polyline: TyyShapeTracked) of Object;

    //-------------------------------------------------------------
    //���ڻ���<�����>��ÿ����һ���������һ�ε��ã�
    //
    //������
    //      polygon             <����>      �������ص�ǰ�����Ƶ�<�����>
    //      ContinueTracking    <���>      �Ƿ�������ƣ�����False�ʹ�����ƽ�����
    //
    //-------------------------------------------------------------
    TEVENTPolygonTrackingHandle = procedure(var Polygon: TyyShapeTracked;
                                            var ContinueTracking: Boolean
                                            ) of Object;

    //-------------------------------------------------------------
    //<�����>�������
    //
    //������
    //      Polygon             <����>      �������������Ƶ�<�����>
    //
    //-------------------------------------------------------------
    TEVENTPolygonTrackedHandle = procedure(var Polygon: TyyShapeTracked) of Object;

    //-------------------------------------------------------------
    //<����>�������
    //
    //������
    //      Rectangle           <����>      �������������Ƶ�<����>
    //
    //-------------------------------------------------------------
    TEVENTRectTrackedHandle = procedure(var Rectangle: TyyRectTracked) of Object;

    //-------------------------------------------------------------
    //<Բ>�������
    //
    //������
    //      Circle              <����>      �������������Ƶ�<Բ>
    //
    //-------------------------------------------------------------
    TEVENTCircleTrackedHandle = procedure(var Circle: TyyCircleTracked) of Object;

    //����Ļ���β�������ʱ��������Ļ�����ƫ��
    TEVENTPanHandle = procedure(X, Y: Integer) of Object;
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//�������ָ�볣����ÿһ��������Ӧyyres.RES�е�һ�������Դ
//------------------------------------------------------------------------------
const MOUSECURSOR_DEFAULT       = 1;
const MOUSECURSOR_ARROW1        = MOUSECURSOR_DEFAULT + 1;
const MOUSECURSOR_ARROW2        = MOUSECURSOR_ARROW1 + 1;
const MOUSECURSOR_CROSS1        = MOUSECURSOR_ARROW2 + 1;
const MOUSECURSOR_CROSS2        = MOUSECURSOR_CROSS1 + 1;
const MOUSECURSOR_CROSSBOX      = MOUSECURSOR_CROSS2 + 1;
const MOUSECURSOR_PICKUP        = MOUSECURSOR_CROSSBOX + 1;
const MOUSECURSOR_MOVE          = MOUSECURSOR_PICKUP + 1;
const MOUSECURSOR_COPY          = MOUSECURSOR_MOVE + 1;
const MOUSECURSOR_DRAG          = MOUSECURSOR_COPY + 1;
const MOUSECURSOR_BEAM          = MOUSECURSOR_DRAG + 1;
const MOUSECURSOR_CANCEL        = MOUSECURSOR_BEAM + 1;
const MOUSECURSOR_MEASURE       = MOUSECURSOR_CANCEL + 1;
const MOUSECURSOR_HAND          = MOUSECURSOR_MEASURE + 1;
const MOUSECURSOR_ZOOM          = MOUSECURSOR_HAND + 1;
const MOUSECURSOR_ZOOMIN        = MOUSECURSOR_ZOOM + 1;
const MOUSECURSOR_ZOOMOUT       = MOUSECURSOR_ZOOMIN + 1;
const MOUSECURSOR_BUSYWAIT      = MOUSECURSOR_ZOOMOUT + 1;
const MOUSECURSOR_BARRIER       = MOUSECURSOR_BUSYWAIT + 1;
const MOUSECURSOR_FOX           = MOUSECURSOR_BARRIER + 1;
const MOUSECURSOR_PIGGY         = MOUSECURSOR_FOX + 1;
const MOUSECURSOR_HOLD          = MOUSECURSOR_PIGGY + 1;
const MOUSECURSOR_CATCH         = MOUSECURSOR_HOLD + 1;
const MOUSECURSOR_PENCIL        = MOUSECURSOR_CATCH + 1;
const MOUSECURSOR_BULLSEYE      = MOUSECURSOR_PENCIL + 1;
const MOUSECURSOR_TERMINATOR    = MOUSECURSOR_BULLSEYE + 1;
const MOUSECURSOR_PALETTE       = MOUSECURSOR_TERMINATOR + 1;
const MAXSPACEDEFINEDCURSORTYPE = MOUSECURSOR_PALETTE; //��unit�������������
//------------------------------------------------------------------------------
type TSpaceCursorType = Integer;    //Space�������״����ֵ�����ϸ������е�һ��


//------------------------------------------------------------------------------
//  ���³���������yySpace�ĸ��ֲ��������ƶ���Ρ�����Բ�εȵȣ�
//------------------------------------------------------------------------------
const SPACETASK_IDLE                    = 0;                        //do nothing
const SPACETASK_TRACKLINE               = SPACETASK_IDLE + 1;       //downup��move��downup
const SPACETASK_TRACKLINE2              = SPACETASK_TRACKLINE + 1;  //down��drag��up
const SPACETASK_TRACKPOLYGON            = SPACETASK_TRACKLINE2 + 1;
const SPACETASK_TRACKRECTANGLE          = SPACETASK_TRACKPOLYGON + 1;   //downup��move��downup
const SPACETASK_TRACKRECTANGLE2         = SPACETASK_TRACKRECTANGLE + 1; //down��drag��up
const SPACETASK_TRACKCIRCLE             = SPACETASK_TRACKRECTANGLE2 + 1;    //downup��move��downup
const SPACETASK_TRACKCIRCLE2            = SPACETASK_TRACKCIRCLE + 1;//down��drag��up
const SPACETASK_PAN                     = SPACETASK_TRACKCIRCLE2 + 1;
//------------------------------------------------------------------------------
type TSpaceTask = Integer;      //sPACE�ĵ�ǰ��������ֵ�����ϸ������е�һ��


//------------------------------------------------------------------------------
//Declaration yySpace
//���¶�����yySpace�Ľṹ
//------------------------------------------------------------------------------
type
TyySpace = class(TWinControl)
  private
//------------------------------------------------------------------------------
    //message handles
    //�¼���Ӧ����
//------------------------------------------------------------------------------
    procedure WMPaint(var Message: TWMPaint);               message WM_PAINT;
    procedure WMSize(var Message: TWMSize);                 message WM_SIZE;
    procedure WMMove(var Message: TWMMove);                 message WM_MOVE;
    procedure WMMouseMove(var Message: TWMMouseMove);       message WM_MOUSEMOVE;
    procedure WMLButtonDown(var Message: TWMLButtonDown);   message WM_LBUTTONDOWN;
    procedure WMRButtonDown(var Message: TWMRButtonDown);   message WM_RBUTTONDOWN;
    procedure WMMButtonDown(var Message: TWMMButtonDown);   message WM_MBUTTONDOWN;
    procedure WMLButtonUp(var Message: TWMLButtonUp);       message WM_LBUTTONUP;
    procedure WMRButtonUp(var Message: TWMRButtonUp);       message WM_RBUTTONUP;
    procedure WMMButtonUp(var Message: TWMMButtonUp);       message WM_MBUTTONUP;
    procedure WMLButtonDblClk(var Message: TWMLButtonDblClk);   message WM_LBUTTONDBLCLK;
    procedure WMRButtonDblClk(var Message: TWMRButtonDblClk);   message WM_RBUTTONDBLCLK;
    procedure WMMButtonDblClk(var Message: TWMMButtonDblClk);   message WM_MBUTTONDBLCLK;
    procedure WMKeyDown(var Message: TWMKeyDown);           message WM_KEYDOWN;
    procedure WMKeyUp(var Message: TWMKeyUp);               message WM_KEYUP;
//------------------------------------------------------------------------------

    //���ڴ�����갴���¼�
    //��WMLButtonDown��WMRButtonDown��WMMButtonDown�����¼���Ӧ��������
    procedure DoMouseDown(Button: TMouseButton;
                          Shift: TShiftState;
                          X: Integer;
                          Y: Integer
                          );

    //���ڴ�������ɿ��¼�
    //��WMLButtonUp��WMRButtonUp��WMMButtonUp�����¼���Ӧ��������
    procedure DoMouseUp(Button: TMouseButton;
                        Shift: TShiftState;
                        X: Integer;
                        Y: Integer
                        );

    //���ڴ������˫���¼�
    //��WMLButtonDblClk��WMRButtonDblClk��WMMButtonDblClk�����¼���Ӧ��������
    procedure DoMouseDoubleClick(Button: TMouseButton;
                                 Shift: TShiftState;
                                 X: Integer;
                                 Y: Integer
                                 );

    //��WMPaint�¼���Ӧ��������
    //�ڴ����ػ�ʱ�ѻ���һ��Ķ������������ڻ������ߣ��ػ�һ��
    procedure OnSelfPaint();

    //�ڻ���<����>ʱ����DoMouseDown��������
    //ÿ����һ�������һ��
    //TEVENTPolylineTrackingHandle�¼�Ҳ�ɱ���������
    procedure TrackLineProcess();

    //��<����>������ɺ����ڽ������Ʋ���
    procedure TrackLineFinish();

    //�ڻ���<�����>ʱ����DoMouseDown��������
    //ÿ����һ�������һ��
    //TEVENTPolygonTrackingHandle�¼�Ҳ�ɱ���������
    procedure TrackPolygonProcess();

    //��<�����>������ɺ����ڽ������Ʋ���
    procedure TrackPolygonFinish();

    //��<����>������ɺ����ڽ������Ʋ���
    procedure TrackRectangleFinish();

    //��<Բ>������ɺ����ڽ������Ʋ���
    procedure TrackCircleFinish();

  protected
//------------------------------------------------------------------------------
    //����������紫���һ��ص����������������¼����ƣ�
//------------------------------------------------------------------------------
    m_EventHandle_Paint             : TEVENTPaintHandle;            //
    m_EventHandle_Size              : TEVENTSizeHandle;             //�����С�ı�
    m_EventHandle_Move              : TEVENTMoveHandle;             //�����ƶ�
    m_EventHandle_MouseDown         : TEVENTMouseDownHandle;        //��������
    m_EventHandle_MouseUp           : TEVENTMouseUpHandle;          //�ɿ�����
    m_EventHandle_MouseMove         : TEVENTMouseMoveHandle;        //����ƶ�
    m_EventHandle_MouseDoubleClick  : TEVENTMouseDoubleClickHandle; //���˫��
    //ע�⵱�����tabstop=falseʱ�������������¼�
    m_EventHandle_KeyDown           : TEVENTKeyDownHandle;          //���¼�
    m_EventHandle_KeyUp             : TEVENTKeyUpHandle;            //�ɿ���
    m_EventHandle_PolylineTracking  : TEVENTPolylineTrackingHandle; //polyline����һ��
    m_EventHandle_PolylineTracked   : TEVENTPolylineTrackedHandle;  //polyline�㶨��
    m_EventHandle_PolygonTracking   : TEVENTPolygonTrackingHandle;  //polygon����һ��
    m_EventHandle_PolygonTracked    : TEVENTPolygonTrackedHandle;   //polygon�㶨��
    m_EventHandle_RectTracked       : TEVENTRectTrackedHandle;      //����д��
    m_EventHandle_CircleTracked     : TEVENTCircleTrackedHandle;    // :(
    m_EventHandle_Pan               : TEVENTPanHandle;
//------------------------------------------------------------------------------

  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy(); override;

//------------------------------------------------------------------------------
    //�������ã�����ע���¼��ص������ĵ�ַ
    //����yySpace��˵���Ǹ�m_EventHandle_*��Ա��ֵ
//------------------------------------------------------------------------------
    function InitialPaintEventHandle(EventHandle: TEVENTPaintHandle): Boolean;
    function InitialSizeEventHandle(EventHandle: TEVENTSizeHandle): Boolean;
    function InitialMoveEventHandle(EventHandle: TEVENTMoveHandle): Boolean;
    function InitialMouseDownEventHandle(EventHandle: TEVENTMouseDownHandle): Boolean;
    function InitialMouseUpEventHandle(EventHandle: TEVENTMouseUpHandle): Boolean;
    function InitialMouseMoveEventHandle(EventHandle: TEVENTMouseMoveHandle): Boolean;
    function InitialMouseDoubleClickEventHandle(EventHandle: TEVENTMouseDoubleClickHandle): Boolean;
    function InitialKeyDownEventHandle(EventHandle: TEVENTKeyDownHandle): Boolean;
    function InitialKeyUpEventHandle(EventHandle: TEVENTKeyUpHandle): Boolean;
    function InitialPolylineEventHandle(DoingEventHandle: TEVENTPolylineTrackingHandle;
                                        DoneEventHandle: TEVENTPolylineTrackedHandle
                                        ): Boolean;
    function InitialPolygonEventHandle(DoingEventHandle: TEVENTPolygonTrackingHandle;
                                       DoneEventHandle: TEVENTPolygonTrackedHandle
                                       ): Boolean;
    function InitialRectEventHandle(DoneHandle: TEVENTRectTrackedHandle): Boolean;
    function InitialCircleEventHandle(DoneHandle: TEVENTCircleTrackedHandle): Boolean;
    function InitialPanEventHandle(PanHandle: TEVENTPanHandle): Boolean;
//------------------------------------------------------------------------------

    //yySpace�ı�����ɫ
    procedure SetBackColor(color: TColor);
    function GetBackColor(): TColor;

    //yySpace�Ĺ����״
    function SetCursor(MouseCursor: TSpaceCursorType): Boolean;
    function GetCursor(): TSpaceCursorType;

    //ȡ���ڴ�dc
    function GetBufferDC(): HDC;

    //����buffer��С
    procedure ResetBuffer();

    //������buff����������ǰ̨
    procedure PostBuffer();

    //����ȡ��yySpace�ĵ�ǰ��������yySpace����idle״̬��
    procedure SetIdleTask();//do nothing
    //���úͻ�ȡyySpace�ĸ���״̬
    function SetTask(SpaceTask: TSpaceTask): Boolean;
    function GetTask(): TSpaceTask;

    procedure DrawFocusEnvelope(const envelope: TRect);
    procedure DrawSelectBox(const rect: TRect; const color: COLORREF);
    procedure DrawEnvelope(envelope: TRect; linecolor: COLORREF; linewidth: Longint;
        hollow: Boolean; fillcolor: COLORREF); overload;
    procedure DrawEnvelope(envelope: TRect; color: COLORREF; linewidth: Longint;
        hollow: Boolean); overload;
    procedure DrawCircle(circle: TyyCircleTracked; linecolor: COLORREF;
        linewidth: Longint; hollow: Boolean; fillcolor: COLORREF); overload;
    procedure DrawCircle(center: tagPOINT; radius: Longint; linecolor: COLORREF;
        linewidth: Longint; hollow: Boolean; fillcolor: COLORREF); overload;
    procedure DrawCircle(center: tagPOINT; radius: Longint; color: COLORREF;
        linewidth: Longint; hollow: Boolean); overload;
    procedure DrawShape(var shape: TyyShapeTracked; linecolor: COLORREF;
        linewidth: Longint; hollow: Boolean; fillcolor: COLORREF);
    procedure DrawPolyline(var shape: TyyShapeTracked; linecolor: COLORREF;
        linewidth: Longint);
    procedure DrawFillPolygon(var shape: TyyShapeTracked; color: COLORREF;
        linewidth: Longint);
    
  private
    m_nSpaceID      : Integer;

    m_DC            : HDC;              //�Լ���dc
    m_MemDC         : HDC;              //��̨�ڴ�dc
    m_BlankDC       : HDC;              //����
    m_PenColor      : TColor;           //����ʱ����ɫ
    m_PenStyle      : Integer;          //����ʱ������
    m_PenWidth      : Integer;          //����ʱ���߿�
    m_BrushColor    : TColor;
    m_BrushStyle    : Integer;

    m_MouseCursor   : TSpaceCursorType; //���ָ����״
    m_CurrentTask   : TSpaceTask;       //��¼yySpace�ĵ�ǰ����״̬

    m_TrackPoints   : array of tagPOINT;//�ڻ��ƣ����ߡ�����εȣ�ʱ����¼�Ѿ����Ƶ�ÿһ����

    //���˽�б���������ʶ����ո��ػ��
    //��Ҫ�ڻ��ƹ����б��������ѿ��ĺۼ�
    m_bJustOnPaint  : Boolean;

  end;
//------------------------------------------------------------------------------

function CreateSpace(Parent: HWND; out yySpace: TyySpace): Boolean;

implementation

{$R yySpace.res}

//�ж�ָ���Ƿ���Ч��nil����False��
function IsValid(p: Pointer): Boolean;
begin
    if (p = nil) then Result := False
    else Result := True;
end;

//�ж�ָ���Ƿ���Ч��nil����True��
function IsInvalid(p: Pointer): Boolean;
begin
    if (p = nil) then Result := True
    else Result := False;
end;

//���ƾ���
function _DrawEnvelope(dc: HDC; point1: tagPOINT; point2: tagPOINT;
    rop2: Integer): Boolean; overload;
var
    rect1: TRect;
    nOldRop2: Integer;
begin
    if (point1.x < point2.x) then begin
        rect1.Left := point1.x;
        rect1.Right := point2.x;
    end
    else begin
        rect1.Left := point2.x;
        rect1.Right := point1.x;
    end;
    if (point1.y < point2.y) then begin
        rect1.Top := Point1.y;
        rect1.Bottom := Point2.y;
    end
    else begin
        rect1.Top := Point2.y;
        rect1.Bottom := Point1.y;
    end;

    nOldRop2 := GetRop2(dc);
    Rectangle(dc, rect1.Left, rect1.Top, rect1.Right, rect1.Bottom);
    SetRop2(dc, nOldRop2);

    Result := False;
end;

function _DrawEnvelope(dc: HDC; envelope: TRect; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF): Boolean; overload;
var
    tagBrush: tagLOGBRUSH;
    BrushNew, BrushOld: HBRUSH;
    PenNew, PenOld: HPEN;
begin

    PenNew := CreatePen(PS_SOLID, linewidth, linecolor);
    PenOld := SelectObject(dc, PenNew);

    if (hollow) then begin
        tagBrush.lbStyle := BS_HOLLOW;
    end
    else begin
        tagBrush.lbStyle := BS_SOLID;
        tagBrush.lbColor := fillcolor
    end;
    BrushNew := CreateBrushIndirect(tagBrush);
    BrushOld := SelectObject(dc, BrushNew);

    Rectangle(dc, envelope.Left, envelope.Top, envelope.Right, envelope.Bottom);

    //�ָ���ǰ��hdc
    SelectObject(dc, BrushOld);
    SelectObject(dc, PenOld);
    DeleteObject(BrushNew);
    DeleteObject(PenNew);

    Result := True;
end;

function _DrawEnvelope(dc: HDC; envelope: TRect; color: COLORREF; linewidth: Longint;
    hollow: Boolean): Boolean; overload;
begin
    Result := _DrawEnvelope(dc, envelope, color, linewidth, hollow, color);
end;

procedure _DrawSelectBox(dc: HDC; envelope: TRect; color: COLORREF);
var
    tagBrush: tagLOGBRUSH;
    BrushNew, BrushOld: HBRUSH;
    PenNew, PenOld: HPEN;
begin

    PenNew := CreatePen(PS_DASH, 1, color);
    PenOld := SelectObject(dc, PenNew);
    tagBrush.lbStyle := BS_HOLLOW;
    BrushNew := CreateBrushIndirect(tagBrush);
    BrushOld := SelectObject(dc, BrushNew);

    Rectangle(dc, envelope.Left, envelope.Top, envelope.Right, envelope.Bottom);

    SelectObject(dc, BrushOld);
    SelectObject(dc, PenOld);
    DeleteObject(BrushNew);
    DeleteObject(PenNew);
end;

//���ƽ������
function _DrawFocusEnvelope(dc: HDC; point1: tagPOINT; point2: tagPOINT; rop2: Integer): Boolean;
var
    rect1: TRect;
    nOldRop2: Integer;
begin
    if (point1.x < point2.x) then begin
        rect1.Left := point1.x;
        rect1.Right := point2.x;
    end
    else begin
        rect1.Left := point2.x;
        rect1.Right := point1.x;
    end;
    if (point1.y < point2.y) then begin
        rect1.Top := Point1.y;
        rect1.Bottom := Point2.y;
    end
    else begin
        rect1.Top := Point2.y;
        rect1.Bottom := Point1.y;
    end;

    nOldRop2 := GetRop2(dc);
    DrawFocusRect(dc, rect1);
    SetRop2(dc, nOldRop2);

    Result := False;
end;

//��Բ
function _DrawCircle(dc: HDC; center: tagPOINT; Point: tagPOINT): Boolean; overload;
var
    tagBrush: tagLOGBRUSH;
    BrushNew, BrushOld: HBRUSH;
    r: Integer; //�뾶
begin

    tagBrush.lbStyle := BS_HOLLOW;
    BrushNew := CreateBrushIndirect(tagBrush);
    BrushOld := SelectObject(dc, BrushNew);

    r := Round(Abs(Sqrt(Sqr(center.x - Point.x) + Sqr(center.y - Point.y))));
    Ellipse(dc,
            center.x - r,
            center.y - r,
            center.x + r,
            center.y + r);

    //�ָ���ǰ��hdc
    SelectObject(dc, BrushOld);
    DeleteObject(BrushNew);

    Result := True;
end;

//��Բ
function _DrawCircle(dc: HDC; center: tagPOINT; Point: tagPOINT; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF): Boolean; overload;
var
    tagBrush: tagLOGBRUSH;
    BrushNew, BrushOld: HBRUSH;
    PenNew, PenOld: HPEN;
    r: Integer; //�뾶
begin
    PenNew := CreatePen(PS_SOLID, linewidth, linecolor);
    PenOld := SelectObject(dc, PenNew);

    if (hollow) then begin
        tagBrush.lbStyle := BS_HOLLOW;
    end
    else begin
        tagBrush.lbStyle := BS_SOLID;
        tagBrush.lbColor := fillcolor
    end;
    BrushNew := CreateBrushIndirect(tagBrush);
    BrushOld := SelectObject(dc, BrushNew);

    r := Round(Abs(Sqrt(Sqr(center.x - Point.x) + Sqr(center.y - Point.y))));
    Ellipse(dc,
            center.x - r,
            center.y - r,
            center.x + r,
            center.y + r);

    //�ָ���ǰ��hdc
    SelectObject(dc, BrushOld);
    SelectObject(dc, PenOld);
    DeleteObject(BrushNew);
    DeleteObject(PenNew);

    Result := True;
end;

//��Բ
function _DrawCircle(dc: HDC; center: tagPOINT; radius: Longint; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF): Boolean; overload;
var
    pnt: tagPOINT;
begin
    pnt.x := center.x + radius;
    pnt.y := center.y;
    Result := _DrawCircle(dc, center, pnt, linecolor, linewidth, hollow, fillcolor);
end;

function _DrawCircle(dc: HDC; center: tagPOINT; radius: Longint; color: COLORREF;
    linewidth: Longint; hollow: Boolean): Boolean; overload;
begin
    Result := _DrawCircle(dc, center, radius, color, linewidth, hollow, color);
end;

//�������
function _DrawShape(dc: HDC; var shape: TyyShapeTracked; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF): Boolean;
var
    tagBrush: tagLOGBRUSH;
    BrushNew, BrushOld: HBRUSH;
    PenNew, PenOld: HPEN;
begin

    PenNew := CreatePen(PS_SOLID, linewidth, linecolor);
    PenOld := SelectObject(dc, PenNew);

    if (hollow) then begin
        tagBrush.lbStyle := BS_HOLLOW;
    end
    else begin
        tagBrush.lbStyle := BS_SOLID;
        tagBrush.lbColor := fillcolor
    end;
    BrushNew := CreateBrushIndirect(tagBrush);
    BrushOld := SelectObject(dc, BrushNew);

    if (hollow) then begin
        Polyline(dc, shape.m_Points, shape.m_PointCount);
    end
    else begin
        Polygon(dc, shape.m_Points, shape.m_PointCount);
    end;       

    //�ָ���ǰ��hdc
    SelectObject(dc, BrushOld);
    SelectObject(dc, PenOld);
    DeleteObject(BrushNew);
    DeleteObject(PenNew);

    Result := True;
end;
//------------------------------------------------------------------------------




constructor TyySpace.Create(AOwner: TComponent);
begin
    inherited Create(AOwner);
    Self.IsControl := False;
    Self.ShowHint := False;
    Self.ParentShowHint := False;
    //��������
//    Self.TabStop := True;   //�ô�����Ի�ý��㣬Ŀ������Ӧkeydown��keyup��Ϣ
    Self.m_bJustOnPaint := False;

    //load cursor resource �������Դ
    Screen.Cursors[MOUSECURSOR_DEFAULT]     := LoadCursor(HInstance, 'IDC_DEFAULT');
    Screen.Cursors[MOUSECURSOR_ARROW1]      := LoadCursor(HInstance, 'IDC_ARROW1');
    Screen.Cursors[MOUSECURSOR_ARROW2]      := LoadCursor(HInstance, 'IDC_ARROW2');
    Screen.Cursors[MOUSECURSOR_CROSS1]      := LoadCursor(HInstance, 'IDC_CROSS1');
    Screen.Cursors[MOUSECURSOR_CROSS2]      := LoadCursor(HInstance, 'IDC_CROSS2');
    Screen.Cursors[MOUSECURSOR_CROSSBOX]    := LoadCursor(HInstance, 'IDC_CROSSBOX');
    Screen.Cursors[MOUSECURSOR_PICKUP]      := LoadCursor(HInstance, 'IDC_PICKUP');
    Screen.Cursors[MOUSECURSOR_MOVE]        := LoadCursor(HInstance, 'IDC_MOVE');
    Screen.Cursors[MOUSECURSOR_COPY]        := LoadCursor(HInstance, 'IDC_COPY');
    Screen.Cursors[MOUSECURSOR_DRAG]        := LoadCursor(HInstance, 'IDC_DRAG');
    Screen.Cursors[MOUSECURSOR_BEAM]        := LoadCursor(HInstance, 'IDC_BEAM');
    Screen.Cursors[MOUSECURSOR_CANCEL]      := LoadCursor(HInstance, 'IDC_CANCEL');
    Screen.Cursors[MOUSECURSOR_MEASURE]     := LoadCursor(HInstance, 'IDC_MEASURE');
    Screen.Cursors[MOUSECURSOR_HAND]        := LoadCursor(HInstance, 'IDC_HAND');
    Screen.Cursors[MOUSECURSOR_ZOOM]        := LoadCursor(HInstance, 'IDC_ZOOM');
    Screen.Cursors[MOUSECURSOR_ZOOMIN]      := LoadCursor(HInstance, 'IDC_ZOOMIN');
    Screen.Cursors[MOUSECURSOR_ZOOMOUT]     := LoadCursor(HInstance, 'IDC_ZOOMOUT');
    Screen.Cursors[MOUSECURSOR_BUSYWAIT]    := LoadCursor(HInstance, 'IDC_BUSYWAIT');
    Screen.Cursors[MOUSECURSOR_BARRIER]     := LoadCursor(HInstance, 'IDC_BARRIER');
    Screen.Cursors[MOUSECURSOR_FOX]         := LoadCursor(HInstance, 'IDC_FOX');
    Screen.Cursors[MOUSECURSOR_PIGGY]       := LoadCursor(HInstance, 'IDC_PIGGY');
    Screen.Cursors[MOUSECURSOR_HOLD]        := LoadCursor(HInstance, 'IDC_HOLD');
    Screen.Cursors[MOUSECURSOR_CATCH]       := LoadCursor(HInstance, 'IDC_CATCH');
    Screen.Cursors[MOUSECURSOR_PENCIL]      := LoadCursor(HInstance, 'IDC_PENCIL');
    Screen.Cursors[MOUSECURSOR_BULLSEYE]    := LoadCursor(HInstance, 'IDC_BULLSEYE');
    Screen.Cursors[MOUSECURSOR_TERMINATOR]  := LoadCursor(HInstance, 'IDC_TERMINATOR');
    Screen.Cursors[MOUSECURSOR_PALETTE]     := LoadCursor(HInstance, 'IDC_PALETTE');

    m_DC        := 0;
    m_MemDC     := 0;
    m_BlankDC   := 0;

end;

destructor TyySpace.Destroy;
begin
    if (Self.m_DC > 0) then begin
        ReleaseDC(Self.Handle, m_DC);
    end;

    if (Self.m_MemDC > 0) then begin
        DeleteDC(m_MemDC);
    end;

    if (Self.m_BlankDC > 0) then begin
        DeleteDC(m_BlankDC);
    end;

    inherited Destroy;
end;

procedure TyySpace.ResetBuffer();
var
    rect: TRect;
    bitmap: HBITMAP;
    brush: HBRUSH;
begin
    if (Self.ParentWindow <= 0) then begin
        Exit;
    end;

    Windows.GetClientRect(Self.ParentWindow, rect);
    Self.Width := rect.Right - rect.Left;
    Self.Height := rect.Bottom - rect.Top;

    bitmap := CreateCompatibleBitmap(Self.m_DC,
                                     Self.Width,
                                     Self.Height
                                     );
    SelectObject(Self.m_MemDC, bitmap);
    DeleteObject(bitmap);

    brush := CreateSolidBrush(Self.Color);
    rect := Self.GetClientRect();
    FillRect(Self.m_MemDC, rect, brush);
    DeleteObject(brush);

    Self.SetBackColor(Self.Color);

end;

function TyySpace.GetBufferDC(): HDC;
begin
    Result := m_MemDC;
end;

procedure TyySpace.PostBuffer();
var
    rect: TRect;
begin                             
    rect := Self.GetClientRect();
    BitBlt(m_DC,
           0,
           0,
           rect.Right - rect.Left,
           rect.Bottom - rect.Top,
           m_MemDC,
           0,
           0,
           SRCCOPY
           );

end;

procedure TyySpace.WMPaint(var Message: TWMPaint);
var
  PS: TPaintStruct;
begin
    if (Message.DC = 0) then begin
        BeginPaint(Handle, PS);
        EndPaint(Handle, PS);
    end;

    Self.PostBuffer();
    Self.OnSelfPaint();
    m_EventHandle_Paint(m_DC);
end;

procedure TyySpace.OnSelfPaint();
var
    LogBrush: TLogBrush;
    brush, OldBrush: HBRUSH;

    NewPen, OldPen: HPEN;
    nOldRop2: Integer;

    i, count: Integer;
begin
    if ((m_CurrentTask = SPACETASK_IDLE)
      or (High(m_TrackPoints) <= 0)) then Exit;

    //��һ������ƶ�ʱ��Ӧ���ٲ�����
    m_bJustOnPaint := True;

    //׼��dc
    LogBrush.lbStyle := BS_SOLID;
    LogBrush.lbColor := Self.Color;
    brush := CreateBrushIndirect(LogBrush);
    FillRect(m_DC, Self.ClientRect, brush);
    DeleteObject(brush);

    LogBrush.lbStyle := m_BrushStyle;
    LogBrush.lbColor := m_BrushColor;
    brush := CreateBrushIndirect(LogBrush);
    OldBrush := SelectObject(m_DC, brush);

    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);
    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

    //�Ѹøɵ��ٸ�һ��
    case m_CurrentTask of
        SPACETASK_TRACKLINE,
        SPACETASK_TRACKLINE2,
        SPACETASK_TRACKPOLYGON: begin
            count := High(m_TrackPoints) + 1;

            MoveToEx(m_DC,
                     m_TrackPoints[0].x,
                     m_TrackPoints[0].y,
                     nil
                     );

            for i := 1 to count - 2 do begin    //ע�����һ���߶�����ʱ�ģ����Բ��û�
                LineTo(m_DC,
                       m_TrackPoints[i].x,
                       m_TrackPoints[i].y
                       );
            end;
        end;
    end;

    //�ɵ���������Ķ���
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
    SelectObject(m_DC, OldBrush);
    DeleteObject(brush);
    SetRop2(m_DC, nOldRop2);
end;

procedure TyySpace.WMSize(var Message: TWMSize);
begin
    if (IsValid(@m_EventHandle_Size)) then begin
        m_EventHandle_Size(Self.Width, Self.Height);
    end;
end;

procedure TyySpace.WMMove(var Message: TWMMove);
begin
    if (IsValid(@m_EventHandle_Move)) then begin
        m_EventHandle_Move();
    end;
end;

procedure TyySpace.DoMouseDown(Button: TMouseButton; Shift: TShiftState;
  X: Integer; Y: Integer);
var
    n: Integer;
begin
    //�ȵ��ûص��������Ȼ��ٸɻ�
    if (IsValid(@m_EventHandle_MouseDown)) then begin
        m_EventHandle_MouseDown(Button,
                                Shift,
                                X,
                                Y);
    end;

    if (Button = mbLeft) then begin
        case m_CurrentTask of
            SPACETASK_TRACKLINE,
            SPACETASK_TRACKPOLYGON: begin
                if (High(m_TrackPoints) < 0) then begin
                    //��һ����
                    SetLength(m_TrackPoints, 2);    //ע������ĵڶ���������mousemove
                    m_TrackPoints[0].x := X;
                    m_TrackPoints[0].y := Y;
                    m_TrackPoints[1].x := X;
                    m_TrackPoints[1].y := Y;

                    if (m_CurrentTask = SPACETASK_TRACKLINE) then begin
                        TrackLineProcess();
                    end
                    else begin
                        TrackPolygonProcess();
                    end;
                end
                else begin
                    //��n����
                    n := High(m_TrackPoints) + 1;
                    SetLength(m_TrackPoints, n + 1);
                    m_TrackPoints[n - 1].x := X;
                    m_TrackPoints[n - 1].y := Y;
                    m_TrackPoints[n].x := X;
                    m_TrackPoints[n].y := Y;
                    if (m_CurrentTask = SPACETASK_TRACKLINE) then begin
                        TrackLineProcess();
                    end
                    else begin
                        TrackPolygonProcess();
                    end;
                end;
            end;

            SPACETASK_TRACKLINE2: begin
                SetLength(m_TrackPoints, 2);
                m_TrackPoints[0].x := X;
                m_TrackPoints[0].y := Y;
                m_TrackPoints[1].x := X;
                m_TrackPoints[1].y := Y;
            end;

            SPACETASK_TRACKRECTANGLE,
            SPACETASK_TRACKCIRCLE,
            SPACETASK_TRACKRECTANGLE2,
            SPACETASK_TRACKCIRCLE2: begin
                if (High(m_TrackPoints) < 0) then begin
                    //��һ����
                    SetLength(m_TrackPoints, 2);    //ע������ĵڶ���������mousemove
                    m_TrackPoints[0].x := X;
                    m_TrackPoints[0].y := Y;
                    m_TrackPoints[1].x := X;
                    m_TrackPoints[1].y := Y;
                end
                else begin
                    //�ڶ�����
                    SetLength(m_TrackPoints, 3);
                    //ע�������m_TrackPoints[2]�����յ㣬
                    //m_TrackPoints[1]����ʱ�㣬��Ҫ����mousemoveʱ���ϵ�rect��
                    //���m_TrackPoints[1] = m_TrackPoints[0]������Ҫ����
                    //������TrackRectangleFinish()�д���
                    m_TrackPoints[2].x := X;
                    m_TrackPoints[2].y := Y;

                    if (m_CurrentTask = SPACETASK_TRACKRECTANGLE) then begin
                        TrackRectangleFinish();
                    end
                    else if (m_CurrentTask = SPACETASK_TRACKCIRCLE) then begin
                        TrackCircleFinish();
                    end;
                end;
            end;

            SPACETASK_PAN: begin
                SetLength(m_TrackPoints, 2);
                m_TrackPoints[0].x := X;
                m_TrackPoints[0].y := Y;
                m_TrackPoints[1].x := X;
                m_TrackPoints[1].y := Y;

            end;

            else begin
                if (High(m_TrackPoints) >= 0) then begin
                    SetLength(m_TrackPoints, 0);
                end;
            end;
        end;
    end
    else if (Button = mbRight) then begin
        case m_CurrentTask of
            SPACETASK_TRACKLINE: begin
                TrackLineFinish();
            end;

            SPACETASK_TRACKPOLYGON: begin
                TrackPolygonFinish();
            end;

            SPACETASK_TRACKRECTANGLE,
            SPACETASK_TRACKCIRCLE: begin
                SetLength(m_TrackPoints, 0);
            end;

            else begin
                if (High(m_TrackPoints) >= 0) then begin
                    SetLength(m_TrackPoints, 0);
                end;
            end;
        end;
    end
    else begin
        //middle button

    end;
end;

procedure TyySpace.DoMouseUp(Button: TMouseButton; Shift: TShiftState;
  X: Integer; Y: Integer);
begin
    //��invoke�ص��������Ȼ��ٸɻ�
    if (IsValid(@m_EventHandle_MouseUp)) then begin
        m_EventHandle_MouseUp(Button,
                              Shift,
                              X,
                              Y);
    end;

    if (Button = mbLeft) then begin
        case m_CurrentTask of
            SPACETASK_TRACKLINE2: begin
                if (High(m_TrackPoints) > 0) then begin
                    SetLength(m_TrackPoints, High(m_TrackPoints) + 2);
                    m_TrackPoints[High(m_TrackPoints)].x := X;
                    m_TrackPoints[High(m_TrackPoints)].y := Y;
                    TrackLineFinish();
                end;
            end;

            SPACETASK_TRACKRECTANGLE2,
            SPACETASK_TRACKCIRCLE2: begin
                if (High(m_TrackPoints) > 0) then begin
                    SetLength(m_TrackPoints, 3);
                    //ע�������m_TrackPoints[2]�����յ㣬
                    m_TrackPoints[2].x := X;
                    m_TrackPoints[2].y := Y;

                    if (m_CurrentTask = SPACETASK_TRACKRECTANGLE2) then begin
                        TrackRectangleFinish();
                    end
                    else begin
                        TrackCircleFinish();
                    end;
                end;
            end;

            SPACETASK_PAN: begin
                if (High(m_TrackPoints) > 0) then begin
                    if (IsValid(@m_EventHandle_Pan)) then begin
                        m_EventHandle_Pan(X - m_TrackPoints[0].x,
                                          Y - m_TrackPoints[0].y
                                          );
                    end;
                    SetLength(m_TrackPoints, 0);
                end;
            end;
            
        end;
    end
    else if (Button = mbRight) then begin
        case m_CurrentTask of
            SPACETASK_TRACKLINE: begin

            end;

            SPACETASK_TRACKPOLYGON: begin

            end;

            SPACETASK_TRACKRECTANGLE: begin

            end;

            SPACETASK_TRACKCIRCLE: begin

            end;

            else begin

            end;
        end;
    end
    else begin
        //middle button

    end;
end;

procedure TyySpace.DoMouseDoubleClick(Button: TMouseButton; Shift: TShiftState;
  X: Integer; Y: Integer);
begin
    if (IsValid(@m_EventHandle_MouseDoubleClick)) then begin
        m_EventHandle_MouseDoubleClick(Button,
                                       Shift,
                                       X,
                                       Y);
    end;
end;

procedure TyySpace.WMMouseMove(var Message: TWMMouseMove);
var
    X, Y: Integer;
    Shift: TShiftState;

    LogBrush: tagLOGBRUSH;
    NewBrush, OldBrush: HBRUSH;
    NewPen, NewPen2, OldPen: HPEN;
    nOldRop2: Integer;
    nLastLine: Integer;

    rect: TRect;
begin
    X := Message.XPos;
    Y := Message.YPos;
    Shift := KeysToShiftState(Message.Keys);

    //�ȸ㶨�ص��������Ժ��ٸɻ�
    if (IsValid(@m_EventHandle_MouseMove)) then begin
        m_EventHandle_MouseMove(Shift,
                                X,
                                Y);
    end;

    //����hdc��pen��׼��������Ƥ��
    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);

    LogBrush.lbStyle := m_BrushStyle;
    LogBrush.lbColor := m_BrushColor;
    NewBrush := CreateBrushIndirect(LogBrush);
    OldBrush := SelectObject(m_DC, NewBrush);

    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

    case m_CurrentTask of
        SPACETASK_TRACKLINE,
        SPACETASK_TRACKLINE2,
        SPACETASK_TRACKPOLYGON: begin
            nLastLine := High(m_TrackPoints) - 1;   //���һ�����
            if (nLastLine >= 0) then begin
                //�����һ��
                //��ʼ�ɻ�
                if (not m_bJustOnPaint) then begin
                    //�ȸɵ��ϴθɵĺۼ�
                    MoveToEx(m_DC,
                             m_TrackPoints[nLastLine].x,
                             m_TrackPoints[nLastLine].y,
                             nil
                             );
                    LineTo(m_DC,
                           m_TrackPoints[nLastLine + 1].x,
                           m_TrackPoints[nLastLine + 1].y
                           );
                    if (m_CurrentTask = SPACETASK_TRACKPOLYGON) then begin
                        //�Ƕ���Σ�Ҫ�໭һ����
                        NewPen2 := CreatePen(PS_DOT, 1, m_PenColor);
                        NewPen := SelectObject(m_DC, NewPen2);
                        LineTo(m_DC,
                               m_TrackPoints[0].x,
                               m_TrackPoints[0].y
                               );
                        SelectObject(m_DC, NewPen);
                        DeleteObject(NewPen2);
                    end;
                end
                else begin
                    m_bJustOnPaint := False;
                end;
                //�ٸ�һ��
                m_TrackPoints[nLastLine + 1].x := X;
                m_TrackPoints[nLastLine + 1].y := Y;
                MoveToEx(m_DC,
                         m_TrackPoints[nLastLine].x,
                         m_TrackPoints[nLastLine].y,
                         nil
                         );
                LineTo(m_DC,
                       m_TrackPoints[nLastLine + 1].x,
                       m_TrackPoints[nLastLine + 1].y
                       );
                if (m_CurrentTask = SPACETASK_TRACKPOLYGON) then begin
                    //�Ƕ���Σ�Ҫ�໭һ����
                    NewPen2 := CreatePen(PS_DOT, 1, m_PenColor);
                    NewPen := SelectObject(m_DC, NewPen2);
                    LineTo(m_DC,
                           m_TrackPoints[0].x,
                           m_TrackPoints[0].y
                           );
                    SelectObject(m_DC, NewPen);
                    DeleteObject(NewPen2);
                end;
            end;
        end;

        SPACETASK_TRACKRECTANGLE,
        SPACETASK_TRACKRECTANGLE2: begin
            if (not m_bJustOnPaint) then begin
                if (High(m_TrackPoints) = 1) then begin
                    Self.PostBuffer();
                    m_TrackPoints[1].x := X;
                    m_TrackPoints[1].y := Y;
                    _DrawEnvelope(m_DC,
                                      m_TrackPoints[0],
                                      m_TrackPoints[1],
                                      R2_XORPEN
                                      );
                end;
            end
            else begin
                m_bJustOnPaint := False;
            end;

        end;

        SPACETASK_TRACKCIRCLE,
        SPACETASK_TRACKCIRCLE2: begin
            if (not m_bJustOnPaint) then begin
                if (High(m_TrackPoints) = 1) then begin
                    Self.PostBuffer();
                    m_TrackPoints[1].x := X;
                    m_TrackPoints[1].y := Y;
                    _DrawCircle(m_DC,
                               m_TrackPoints[0],
                               m_TrackPoints[1]
                               );
                end;
            end
            else begin
                m_bJustOnPaint := False;
            end;

        end;

        SPACETASK_PAN: begin
            if (High(m_TrackPoints) > 0) then begin
                m_TrackPoints[1].x := X;
                m_TrackPoints[1].y := Y;

                rect := Self.GetClientRect();
                //�����õ�������λͼ
                if (Self.m_BlankDC > 0) then begin
                    NewBrush := CreateSolidBrush(clSilver);
                    FillRect(Self.m_BlankDC, rect, NewBrush);
                    DeleteObject(NewBrush);

                    BitBlt(m_BlankDC,
                           X - m_TrackPoints[0].x,
                           Y - m_TrackPoints[0].y,
                           rect.Right - rect.Left,
                           rect.Bottom - rect.Top,
                           m_MemDC,
                           0,
                           0,
                           SRCCOPY
                           );

                    BitBlt(m_DC,
                           0,
                           0,
                           rect.Right - rect.Left,
                           rect.Bottom - rect.Top,
                           m_BlankDC,
                           0,
                           0,
                           SRCCOPY
                           );
                end
                else begin
                    BitBlt(m_DC,
                           X - m_TrackPoints[0].x,
                           Y - m_TrackPoints[0].y,
                           rect.Right - rect.Left,
                           rect.Bottom - rect.Top,
                           m_MemDC,
                           0,
                           0,
                           SRCCOPY
                           );
                end;


            end;

        end;

        else begin

        end;
    end;

    //�ɵ���������Ķ���
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
    SelectObject(m_DC, OldBrush);
    DeleteObject(NewBrush);
    SetRop2(m_DC, nOldRop2);
end;

procedure TyySpace.WMLButtonDown(var Message: TWMLButtonDown);
begin
    DoMouseDown(mbLeft,
                KeysToShiftState(Message.Keys),
                Message.XPos,
                Message.YPos);
end;

procedure TyySpace.WMRButtonDown(var Message: TWMRButtonDown);
begin
    DoMouseDown(mbRight,
                KeysToShiftState(Message.Keys),
                Message.XPos,
                Message.YPos);
end;

procedure TyySpace.WMMButtonDown(var Message: TWMMButtonDown);
begin
    DoMouseDown(mbMiddle,
                KeysToShiftState(Message.Keys),
                Message.XPos,
                Message.YPos);
end;

procedure TyySpace.WMLButtonUp(var Message: TWMLButtonUp);
begin
    DoMouseUp(mbLeft,
              KeysToShiftState(Message.Keys),
              Message.XPos,
              Message.YPos);
end;

procedure TyySpace.WMRButtonUp(var Message: TWMRButtonUp);
begin
    DoMouseUp(mbRight,
              KeysToShiftState(Message.Keys),
              Message.XPos,
              Message.YPos);
end;

procedure TyySpace.WMMButtonUp(var Message: TWMMButtonUp);
begin
    DoMouseUp(mbMiddle,
              KeysToShiftState(Message.Keys),
              Message.XPos,
              Message.YPos);
end;

procedure TyySpace.WMLButtonDblClk(var Message: TWMLButtonDblClk);
begin
    DoMouseDoubleClick(mbLeft,
                       KeysToShiftState(Message.Keys),
                       Message.XPos,
                       Message.YPos);
end;

procedure TyySpace.WMRButtonDblClk(var Message: TWMRButtonDblClk);
begin
    DoMouseDoubleClick(mbRight,
                       KeysToShiftState(Message.Keys),
                       Message.XPos,
                       Message.YPos);
end;

procedure TyySpace.WMMButtonDblClk(var Message: TWMMButtonDblClk);
begin
    DoMouseDoubleClick(mbMiddle,
                       KeysToShiftState(Message.Keys),
                       Message.XPos,
                       Message.YPos);
end;

procedure TyySpace.WMKeyDown(var Message: TWMKeyDown);
var
  ShiftState: TShiftState;
begin
    if (IsValid(@m_EventHandle_KeyDown)) then begin
        ShiftState := KeyDataToShiftState(Message.KeyData);
        m_EventHandle_KeyDown(Message.CharCode,
                              ShiftState);
    end;
end;

procedure TyySpace.WMKeyUp(var Message: TWMKeyUp);
var
  ShiftState: TShiftState;
begin
    if (IsValid(@m_EventHandle_KeyUp)) then begin
        ShiftState := KeyDataToShiftState(Message.KeyData);
        m_EventHandle_KeyUp(Message.CharCode,
                            ShiftState);
    end;
end;

procedure TyySpace.TrackLineProcess();
var
    yyPolyline: TyyShapeTracked;
    i: Integer;
    bContinueTrack: Boolean;
begin
    if (IsInvalid(@m_EventHandle_PolylineTracking)) then Exit;

    yyPolyline.m_PointCount := High(m_TrackPoints); //ע��m_TrackPoints�����һ������Ч��
    if (yyPolyline.m_PointCount > MAXPOINTSINPOLYLINE) then begin
        //���������ܴ���MAXPOINTSINPOLYLINE
        yyPolyline.m_PointCount := MAXPOINTSINPOLYLINE;
    end;
    for i := 0 to yyPolyline.m_PointCount - 1 do begin
        yyPolyline.m_Points[i].x := m_TrackPoints[i].x;
        yyPolyline.m_Points[i].y := m_TrackPoints[i].y;
    end;
    m_EventHandle_PolylineTracking(yyPolyline, bContinueTrack);

    if (not bContinueTrack) then begin
        TrackLineFinish();
    end;

end;

procedure TyySpace.TrackLineFinish();
var
    yyPolyline: TyyShapeTracked;
    i: Integer;
    NewPen, OldPen: HPEN;
    nOldRop2: Integer;
begin
    Self.PostBuffer();

    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);
    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

//    count := High(m_TrackPoints) + 1;   //ע��������һ������ĵ�

    if (High(m_TrackPoints) < 2) then begin
        //�߲�������2���㣬ע��������һ������ʱ��
        if (High(m_TrackPoints) >= 0) then begin
            if (not m_bJustOnPaint) then begin
                //�������ƹ���
(*
                MoveToEx(m_DC,
                         m_TrackPoints[0].x,
                         m_TrackPoints[0].y,
                         nil
                         );
                LineTo(m_DC,
                       m_TrackPoints[1].x,
                       m_TrackPoints[1].y
                       );
*)
            end;
            SetLength(m_TrackPoints, 0);
        end;

        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);
        Exit;
    end;

    if (not m_bJustOnPaint) then begin
        //�������һ����
(*
        MoveToEx(m_DC,
                 m_TrackPoints[count - 2].x,
                 m_TrackPoints[count - 2].y,
                 nil
                 );
        LineTo(m_DC,
               m_TrackPoints[count - 1].x,
               m_TrackPoints[count - 1].y
               );
*)
    end
    else begin
        m_bJustOnPaint := False;
    end;

    //--------------------------------------------------------------------------
    //������polyline�Ѿ������ˣ�ע����n-1���㣬���һ�����Ƕ����
    //--------------------------------------------------------------------------

    if (IsInvalid(@m_EventHandle_PolylineTracked)) then begin
        SetLength(m_TrackPoints, 0);
        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);

        Exit;
    end;

    SetLength(m_TrackPoints, High(m_TrackPoints));  //���һ��������ʱ��

    yyPolyline.m_PointCount := High(m_TrackPoints) + 1;
    if (yyPolyline.m_PointCount > MAXPOINTSINPOLYLINE) then begin
        //���������ܴ���MAXPOINTSINPOLYLINE
        yyPolyline.m_PointCount := MAXPOINTSINPOLYLINE;
    end;
    for i := 0 to yyPolyline.m_PointCount - 1 do begin
        yyPolyline.m_Points[i].x := m_TrackPoints[i].x;
        yyPolyline.m_Points[i].y := m_TrackPoints[i].y;
    end;
    m_EventHandle_PolylineTracked(yyPolyline);

    if (m_CurrentTask <> SPACETASK_TRACKLINE) then begin
        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);

        Exit;
    end;

    SetRop2(m_DC, R2_COPYPEN);

(*
    MoveToEx(m_DC,
             m_TrackPoints[0].x,
             m_TrackPoints[0].y,
             nil
             );

    for i := 1 to High(m_TrackPoints) do begin
        LineTo(m_DC,
               m_TrackPoints[i].x,
               m_TrackPoints[i].y
               );
    end;
*)
    //�ɵ���������Ķ���
    SetLength(m_TrackPoints, 0);
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
    SetRop2(m_DC, nOldRop2);

end;

procedure TyySpace.TrackPolygonProcess();
var
    yyPolygon: TyyShapeTracked;
    i: Integer;
    bContinueTrack: Boolean;
begin
    if (IsInvalid(@m_EventHandle_PolygonTracking)) then Exit;

    yyPolygon.m_PointCount := High(m_TrackPoints); //ע��m_TrackPoints�����һ������Ч��
    if (yyPolygon.m_PointCount > MAXPOINTSINPOLYGON) then begin
        //���������ܴ���MAXPOINTSINPOLYGON
        yyPolygon.m_PointCount := MAXPOINTSINPOLYGON;
    end;
    for i := 0 to yyPolygon.m_PointCount - 1 do begin
        yyPolygon.m_Points[i].x := m_TrackPoints[i].x;
        yyPolygon.m_Points[i].y := m_TrackPoints[i].y;
    end;
    m_EventHandle_PolygonTracking(yyPolygon, bContinueTrack);

    if (not bContinueTrack) then begin
        TrackPolygonFinish();
    end;
end;

procedure TyySpace.TrackPolygonFinish();
var
    yyPolygon: TyyShapeTracked;
    i: Integer;
    NewPen, NewPen2, OldPen: HPEN;
    nOldRop2: Integer;
    logBrush: tagLOGBRUSH;
    NewBrush, OldBrush: HBRUSH;
begin
    Self.PostBuffer();

    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);
    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

    logBrush.lbStyle := m_BrushStyle;
    logBrush.lbColor := m_BrushColor;
    NewBrush := CreateBrushInDirect(logBrush);
    OldBrush := SelectObject(m_DC, NewBrush);

//    count := High(m_TrackPoints) + 1;   //ע��������һ������ĵ�

    if (High(m_TrackPoints) < 3) then begin
        //����β�������3���㣬ע��������һ������ʱ��
        if (High(m_TrackPoints) >= 0) then begin
            if (not m_bJustOnPaint) then begin
                //�������ƹ���
(*
                MoveToEx(m_DC,
                         m_TrackPoints[0].x,
                         m_TrackPoints[0].y,
                         nil
                         );
                for i := 1 to count - 1 do begin
                    LineTo(m_DC,
                           m_TrackPoints[i].x,
                           m_TrackPoints[i].y
                           );
                end;
*)
                NewPen2 := CreatePen(PS_DOT, 1, m_PenColor);
                NewPen := SelectObject(m_DC, NewPen2);
(*
                LineTo(m_DC,
                       m_TrackPoints[0].x,
                       m_TrackPoints[0].y
                       );
*)
                SelectObject(m_DC, NewPen);
                DeleteObject(NewPen2);
            end;
            SetLength(m_TrackPoints, 0);
        end;

        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);
        SelectObject(m_DC, OldBrush);
        DeleteObject(NewBrush);
        Exit;
    end;

    if (not m_bJustOnPaint) then begin
        //�������������
(*
        MoveToEx(m_DC,
                 m_TrackPoints[count - 2].x,
                 m_TrackPoints[count - 2].y,
                 nil
                 );
        LineTo(m_DC,
               m_TrackPoints[count - 1].x,
               m_TrackPoints[count - 1].y
               );
*)
        NewPen2 := CreatePen(PS_DOT, 1, m_PenColor);
        NewPen := SelectObject(m_DC, NewPen2);
        LineTo(m_DC,
               m_TrackPoints[0].x,
               m_TrackPoints[0].y
               );
        SelectObject(m_DC, NewPen);
        DeleteObject(NewPen2);
        //�պ�
(*
        LineTo(m_DC,
               m_TrackPoints[count - 2].x,
               m_TrackPoints[count - 2].y
               );
*)
    end;

    //--------------------------------------------------------------------------
    //������polygon�Ѿ������ˣ�ע����n-1���㣬���һ�����Ƕ����
    //--------------------------------------------------------------------------

    if (IsInvalid(@m_EventHandle_PolygonTracked)) then begin
        //ע���������һ���㱻���λ��ƣ����ձ��ɵ��ˣ�����Ҫ����
(*
        MoveToEx(m_DC,
                 m_TrackPoints[0].x,
                 m_TrackPoints[0].y,
                 nil
                 );
        LineTo(m_DC,
               m_TrackPoints[0].x + 1,
               m_TrackPoints[0].y + 1
               );
        MoveToEx(m_DC,
                 m_TrackPoints[High(m_TrackPoints) - 1].x,
                 m_TrackPoints[High(m_TrackPoints) - 1].y,
                 nil
                 );
        LineTo(m_DC,
               m_TrackPoints[High(m_TrackPoints) - 1].x + 1,
               m_TrackPoints[High(m_TrackPoints) - 1].y + 1
               );
*)
        SetLength(m_TrackPoints, 0);
        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);
        Exit;
    end;

    SetLength(m_TrackPoints, High(m_TrackPoints));  //���һ��������ʱ��
    yyPolygon.m_PointCount := High(m_TrackPoints) + 1;
    if (yyPolygon.m_PointCount > MAXPOINTSINPOLYGON) then begin
        //���������ܴ���MAXPOINTSINPOLYLINE
        yyPolygon.m_PointCount := MAXPOINTSINPOLYGON;
    end;
    for i := 0 to yyPolygon.m_PointCount - 1 do begin
        yyPolygon.m_Points[i].x := m_TrackPoints[i].x;
        yyPolygon.m_Points[i].y := m_TrackPoints[i].y;
    end;
    m_EventHandle_PolygonTracked(yyPolygon);

    if (m_CurrentTask <> SPACETASK_TRACKPOLYGON) then begin
        SetLength(m_TrackPoints, 0);
        //�ɵ���������Ķ���
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);
        Exit;
    end;

(*
    BeginPath(m_DC);
    MoveToEx(m_DC,
             m_TrackPoints[0].x,
             m_TrackPoints[0].y,
             nil
             );
    for i := 1 to High(m_TrackPoints) do begin
        LineTo(m_DC,
               m_TrackPoints[i].x,
               m_TrackPoints[i].y
               );
    end;
    LineTo(m_DC,
           m_TrackPoints[0].x,
           m_TrackPoints[0].y
           );
    EndPath(m_DC);
    nFillRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_COPYPEN);
    FillPath(m_DC);
*)
    (*
    //�߿�
    MoveToEx(m_DC,
             m_TrackPoints[0].x,
             m_TrackPoints[0].y,
             nil
             );
    for i := 1 to High(m_TrackPoints) do begin
        LineTo(m_DC,
               m_TrackPoints[i].x,
               m_TrackPoints[i].y
               );
    end;
    LineTo(m_DC,
           m_TrackPoints[0].x,
           m_TrackPoints[0].y
           );
    *)

//    SetRop2(m_DC, nFillRop2);
    //�ɵ���������Ķ���
    SetLength(m_TrackPoints, 0);
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
    SelectObject(m_DC, OldBrush);
    DeleteObject(NewBrush);
    SetRop2(m_DC, nOldRop2);
end;

procedure TyySpace.TrackRectangleFinish();
var
    yyRect: TyyRectTracked;
    NewPen, OldPen: HPEN;
    nOldRop2: Integer;
begin
    if (High(m_TrackPoints) <> 2) then begin
        //ȥ����
        SetLength(m_TrackPoints, 0);
        Exit;
    end;

    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);
    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

    //�ɵ����һ��mousemove
    _DrawEnvelope(
        m_DC,
        m_TrackPoints[0],
        m_TrackPoints[1],
        R2_XORPEN);
    //������ȷ��
    _DrawEnvelope(
        m_DC,
        m_TrackPoints[0],
        m_TrackPoints[2],
        R2_XORPEN);

    //���ûص�
    if (IsValid(@m_EventHandle_RectTracked)) then begin
        yyRect.m_Point1 := m_TrackPoints[0];
        yyRect.m_Point2 := m_TrackPoints[2];
        m_EventHandle_RectTracked(yyRect);
    end;

    if (m_CurrentTask <> SPACETASK_TRACKRECTANGLE) then begin
        //�ɵ���������Ķ���
        SetLength(m_TrackPoints, 0);
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
        SetRop2(m_DC, nOldRop2);
        Exit;
    end;

    //�ɵ���������Ķ���
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
    SetRop2(m_DC, nOldRop2);
    SetLength(m_TrackPoints, 0);

end;

procedure TyySpace.TrackCircleFinish();
var
    yyCircle: TyyCircleTracked;

    NewPen, OldPen: HPEN;
    nOldRop2: Integer;
//    logBrush: tagLOGBRUSH;
//    NewBrush, OldBrush: HBRUSH;
begin
    if (High(m_TrackPoints) <> 2) then begin
        //ȥ����
        SetLength(m_TrackPoints, 0);
        Exit;
    end;

    NewPen := CreatePen(m_PenStyle, m_PenWidth, m_PenColor);
    OldPen := SelectObject(m_DC, NewPen);
    nOldRop2 := GetRop2(m_DC);
    SetRop2(m_DC, R2_XORPEN);//���ʽ

//    logBrush.lbStyle := m_BrushStyle;
//    logBrush.lbColor := m_BrushColor;
//    NewBrush := CreateBrushInDirect(logBrush);
//    OldBrush := SelectObject(m_DC, NewBrush);

    //�ɵ����һ��mousemove
    _DrawCircle(m_DC,
               m_TrackPoints[0],
               m_TrackPoints[1]
               );
    //���������
    SetRop2(m_DC, R2_COPYPEN);
//    BeginPath(m_DC);
    _DrawCircle(m_DC,
               m_TrackPoints[0],
               m_TrackPoints[2]
               );
//    EndPath(m_DC);

    if (Isvalid(@m_EventHandle_CircleTracked)) then begin
        yyCircle.m_Center := m_TrackPoints[0];
        yyCircle.m_Radius := Round(Abs(Sqrt(Sqr(m_TrackPoints[0].x - m_TrackPoints[2].x)
          + Sqr(m_TrackPoints[0].y - m_TrackPoints[2].y))));
        m_EventHandle_CircleTracked(yyCircle);
    end;

    if (m_CurrentTask <> SPACETASK_TRACKCIRCLE) then begin
        SetLength(m_TrackPoints, 0);
        SelectObject(m_DC, OldPen);
        DeleteObject(NewPen);
//        SelectObject(m_DC, OldBrush);
//        DeleteObject(NewBrush);
        SetRop2(m_DC, nOldRop2);
        Exit;
    end;

    //�ɵ���������Ķ���
    SetLength(m_TrackPoints, 0);
    SelectObject(m_DC, OldPen);
    DeleteObject(NewPen);
//    SelectObject(m_DC, OldBrush);
//    DeleteObject(NewBrush);
    SetRop2(m_DC, nOldRop2);
end;

procedure TyySpace.DrawFocusEnvelope(const envelope: TRect);
begin
    _DrawFocusEnvelope(m_MemDC, envelope.TopLeft, envelope.BottomRight, R2_COPYPEN);
end;

procedure TyySpace.DrawSelectBox(const rect: TRect; const color: COLORREF);
begin
    _DrawSelectBox(m_MemDC, rect, color);
end;

procedure TyySpace.DrawEnvelope(envelope: TRect; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF);
begin
    _DrawEnvelope(m_MemDC, envelope, linecolor, linewidth, hollow, fillcolor);
end;

procedure TyySpace.DrawEnvelope(envelope: TRect; color: COLORREF; linewidth: Longint;
    hollow: Boolean);
begin
    _DrawEnvelope(m_MemDC, envelope, color, linewidth, hollow);
end;

procedure TyySpace.DrawCircle(circle: TyyCircleTracked; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF);
begin
    _DrawCircle(m_MemDC, circle.m_Center, circle.m_Radius, linecolor,
        linewidth, hollow, fillcolor);
end;

procedure TyySpace.DrawCircle(center: tagPOINT; radius: Longint; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF);
begin
    _DrawCircle(m_MemDC, center, radius, linecolor, linewidth, hollow, fillcolor);
end;

procedure TyySpace.DrawCircle(center: tagPOINT; radius: Longint; color: COLORREF;
    linewidth: Longint; hollow: Boolean);
begin
    _DrawCircle(m_MemDC, center, radius, color, linewidth, hollow);
end;

procedure TyySpace.DrawShape(var shape: TyyShapeTracked; linecolor: COLORREF;
    linewidth: Longint; hollow: Boolean; fillcolor: COLORREF);
begin
    _DrawShape(m_MemDC, shape, linecolor, linewidth, hollow, fillcolor);
end;

procedure TyySpace.DrawPolyline(var shape: TyyShapeTracked; linecolor: COLORREF;
    linewidth: Longint);
begin
    _DrawShape(m_MemDC, shape, linecolor, linewidth, True, linecolor);
end;

procedure TyySpace.DrawFillPolygon(var shape: TyyShapeTracked; color: COLORREF;
    linewidth: Longint);
begin
    _DrawShape(m_MemDC, shape, color, linewidth, False, color);
end;

procedure TyySpace.SetIdleTask();
begin
    m_CurrentTask := SPACETASK_IDLE;
    SetLength(m_TrackPoints, 0);
end;

function TyySpace.SetTask(SpaceTask: TSpaceTask): Boolean;
begin
    Result := False;

    case SpaceTask of
        SPACETASK_IDLE: begin
            SetIdleTask();
        end;

        SPACETASK_TRACKLINE: begin
            if (m_CurrentTask <> SPACETASK_TRACKLINE) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKLINE;
            end;
        end;

        SPACETASK_TRACKLINE2: begin
            if (m_CurrentTask <> SPACETASK_TRACKLINE2) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKLINE2;
            end;
        end;

        SPACETASK_TRACKPOLYGON: begin
            if (m_CurrentTask <> SPACETASK_TRACKPOLYGON) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKPOLYGON;
            end;
        end;

        SPACETASK_TRACKRECTANGLE: begin
            if (m_CurrentTask <> SPACETASK_TRACKRECTANGLE) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKRECTANGLE;
            end;
        end;

        SPACETASK_TRACKRECTANGLE2: begin
            if (m_CurrentTask <> SPACETASK_TRACKRECTANGLE2) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKRECTANGLE2;
            end;
        end;

        SPACETASK_TRACKCIRCLE: begin
            if (m_CurrentTask <> SPACETASK_TRACKCIRCLE) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKCIRCLE;
            end;
        end;

        SPACETASK_TRACKCIRCLE2: begin
            if (m_CurrentTask <> SPACETASK_TRACKCIRCLE2) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_TRACKCIRCLE2;
            end;
        end;

        SPACETASK_PAN: begin
            if (m_CurrentTask <> SPACETASK_PAN) then begin
                SetIdleTask();
                m_CurrentTask := SPACETASK_PAN;
            end;
        end;

        else begin
            Exit;
        end;
    end;
end;

function TyySpace.GetTask(): TSpaceTask;
begin
    Result := m_CurrentTask;
end;



function TyySpace.InitialPaintEventHandle(EventHandle: TEVENTPaintHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_Paint := EventHandle;
    Result := True;
end;

function TyySpace.InitialSizeEventHandle(EventHandle: TEVENTSizeHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_Size := EventHandle;
    Result := True;
end;

function TyySpace.InitialMoveEventHandle(EventHandle: TEVENTMoveHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_Move := EventHandle;
    Result := True;
end;

function TyySpace.InitialMouseDownEventHandle(EventHandle: TEVENTMouseDownHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_MouseDown := EventHandle;
    Result := True;
end;

function TyySpace.InitialMouseUpEventHandle(EventHandle: TEVENTMouseUpHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_MouseUp := EventHandle;
    Result := True;
end;

function TyySpace.InitialMouseMoveEventHandle(EventHandle: TEVENTMouseMoveHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_MouseMove := EventHandle;
    Result := True;
end;

function TyySpace.InitialMouseDoubleClickEventHandle(EventHandle:
  TEVENTMouseDoubleClickHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_MouseDoubleClick := EventHandle;
    Result := True;
end;

function TyySpace.InitialKeyDownEventHandle(EventHandle: TEVENTKeyDownHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_KeyDown := EventHandle;
    Result := True;
end;

function TyySpace.InitialKeyUpEventHandle(EventHandle: TEVENTKeyUpHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@EventHandle)) then begin
        Exit;
    end;
    m_EventHandle_KeyUp := EventHandle;
    Result := True;
end;

function TyySpace.InitialPolylineEventHandle(DoingEventHandle: TEVENTPolylineTrackingHandle;
  DoneEventHandle: TEVENTPolylineTrackedHandle): Boolean;
begin
(*
    Result := False;
    if ((IsInvalid(@DoingEventHandle))
      or (IsInvalid(@DoneEventHandle))) then begin
        Exit;
    end;
*)
    m_EventHandle_PolylineTracking := DoingEventHandle;
    m_EventHandle_PolylineTracked := DoneEventHandle;
    Result := True;
end;

function TyySpace.InitialPolygonEventHandle(DoingEventHandle: TEVENTPolygonTrackingHandle;
  DoneEventHandle: TEVENTPolygonTrackedHandle): Boolean;
begin
(*
    Result := False;
    if ((IsInvalid(@DoingEventHandle))
      or (IsInvalid(@DoneEventHandle))) then begin
        Exit;
    end;
*)
    m_EventHandle_PolygonTracking := DoingEventHandle;
    m_EventHandle_PolygonTracked := DoneEventHandle;
    Result := True;
end;

function TyySpace.InitialRectEventHandle(DoneHandle: TEVENTRectTrackedHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@DoneHandle)) then begin
        Exit;
    end;

    m_EventHandle_RectTracked := DoneHandle;
    Result := True;
end;

function TyySpace.InitialCircleEventHandle(DoneHandle: TEVENTCircleTrackedHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@DoneHandle)) then begin
        Exit;
    end;

    m_EventHandle_CircleTracked := DoneHandle;
    Result := True;
end;

function TyySpace.InitialPanEventHandle(PanHandle: TEVENTPanHandle): Boolean;
begin
    Result := False;
    if (IsInvalid(@PanHandle)) then begin
        Exit;
    end;

    m_EventHandle_Pan := PanHandle;
    Result := True;
end;

procedure TyySpace.SetBackColor(color: TColor);
var
    bitmap: HBITMAP;
    brush: HBRUSH;
begin
    Self.Color := color;
    if (m_BlankDC > 0) then begin
        DeleteDC(m_BlankDC);
    end;

    m_BlankDC := CreateCompatibleDC(Self.m_DC);
    bitmap := CreateCompatibleBitmap(Self.m_DC,
                                     Self.GetClientRect().Right - Self.GetClientRect().Left,
                                     Self.GetClientRect().Bottom - Self.GetClientRect().Top
                                     );
    SelectObject(m_BlankDC, bitmap);
    DeleteObject(bitmap);

    brush := CreateSolidBrush(Self.Color);
    FillRect(m_BlankDC, Self.GetClientRect(), brush);
    DeleteObject(brush);
end;

function TyySpace.GetBackColor(): TColor;
begin
    Result := Self.Color;
end;

function TyySpace.SetCursor(MouseCursor: TSpaceCursorType): Boolean;
begin
    Result := False;
    if ((MouseCursor < 1) or (MouseCursor > MAXSPACEDEFINEDCURSORTYPE)) then begin
        Exit;
    end;

    m_MouseCursor := MouseCursor;
    Self.Cursor := m_MouseCursor;

    Result := True;
end;

function TyySpace.GetCursor(): TSpaceCursorType;
begin
    Result := m_MouseCursor;
end;


function CreateSpace(Parent: HWND; out yySpace: TyySpace): Boolean;
var
    ParentRect: TRect;
begin
    Result := False;
    if (Parent <= 0) then begin
        Exit;
    end;

    yySpace := TyySpace.Create(nil);
    yySpace.ParentWindow := Parent;
    yySpace.Left := 0;
    yySpace.Top := 0;
    GetWindowRect(Parent, ParentRect);
    yySpace.Width := ParentRect.Right - ParentRect.Left;
    yySpace.Height := ParentRect.Bottom - ParentRect.Top;
    yySpace.m_nSpaceID := 0;

    SetLength(yySpace.m_TrackPoints, 0);
    yySpace.m_DC := GetDC(yySpace.Handle);
    yySpace.m_PenColor := clYellow;
    yySpace.m_PenStyle := PS_SOLID;
    yySpace.m_PenWidth := 1;
    yySpace.m_BrushColor := clWhite;
    yySpace.m_BrushStyle := BS_NULL;

    yySpace.m_MemDC := CreateCompatibleDC(yySpace.m_DC);
    yySpace.SetBackColor(clWhite);

    Result := True;
end;


initialization

finalization

end.
