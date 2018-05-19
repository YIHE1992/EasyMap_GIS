unit InterfaceMap;

interface

uses
    InterfaceObj, InterfaceDisplay, InterfaceLayer, WKSStructs, Windows;

type
    TBookMarkStyle = Longint;
const
    BOOKMARKSTYLE_RECT      = TBookMarkStyle(0);
    BOOKMARKSTYLE_ROUNDRECT = TBookMarkStyle(1);
    BOOKMARKSTYLE_ELLIPSE   = TBookMarkStyle(2);

type
    TBookMarkVisible = Longint;
const
    BOOKMARKVISIBLE_NONE    = TBookMarkVisible(0);
    BOOKMARKVISIBLE_ALL     = TBookMarkVisible(1);
    BOOKMARKVISIBLE_CURRENT = TBookMarkVisible(2);

type IMap = interface(IPersist)
    function AddLayer(const pLayer: ILayer): Boolean; stdcall;
    function DeleteLayer(const index: Longword): Boolean; stdcall;
    function DeleteLayerEx(pLayer: ILayer): Boolean; stdcall;
    function GetLayer(out ppLayer: ILayer; const index: Longword): Boolean; stdcall;
    function SetLayerOrder(const pLayer: ILayer; const neworder: Longword): Boolean; stdcall;
    procedure ClearLayers(); stdcall;
    function GetLayerCount(): Longword; stdcall;
    function GetAllCount(): Longword; stdcall;
    function FindLayer(out ppLayer: ILayer; const layername: PChar;
        const classtype: PChar): Boolean; stdcall;
    procedure ClearAllData(); stdcall;

    function GetReferenceScale(out refscale: Double): Boolean; stdcall;
    function GetSpatialReference(): PChar; stdcall;

    function GetMapScale(out scale: Double): Boolean; stdcall;
    function GetVisibleExtent(out extent: WKSRect): Boolean; stdcall;

    //ȡ�õ�ǰ�����ص����ݵ����Χ
    function GetFullExtent(out fullext: WKSRect): Boolean; stdcall;

    function SetSelectSymbol(const pSymbol: ISymbol): Boolean; stdcall;
    function GetSelectSymbol(const symboltype: TSymbolType;
        out ppSymbol: ISymbol): Boolean; stdcall;

    //����Щ
    function SelectByPoint(var point: WKSPoint; const append: Boolean): Longword; stdcall;
    function Select(var envelope: WKSRect; const partialselect: Boolean; const append: Boolean): Longword; stdcall;
    function DeselectByPoint(var point: WKSPoint): Longword; stdcall;
    function Deselect(var envelope: WKSRect; const partialselect: Boolean): Longword; stdcall;
    procedure ClearSelection(); stdcall;

    procedure SetName(const mapname: PChar);  stdcall;
    function GetName(): PChar; stdcall;

    //ͼ���б��ͳһ�༭����
    function SetUndoPoint(const desc: PChar): Boolean; stdcall;
    function EditUndoable(): Boolean; stdcall;
    function EditRedoable(): Boolean; stdcall;
    function EditUndo(): Boolean; stdcall;
    function EditRedo(): Boolean; stdcall;
    function EditCancel(): Boolean; stdcall;
    function SaveData(): Boolean; stdcall;
    function IsDirty(): Boolean; stdcall;
end;

type IRapidDraw = interface(IPersist)
    //����һ��ͼ�㣬�������ػ��ʱ���draw
    function RD_AddLayer(const pLayer: ILayer): Boolean; stdcall;
    function RD_RemoveLayer(const index: Longword): Boolean; stdcall;
    function RD_RemoveLayerEx(pLayer: ILayer): Boolean; stdcall;
    function RD_GetLayer(out ppLayer: ILayer; const index: Longword): Boolean; stdcall;
    function RD_SetLayerOrder(const pLayer: ILayer; const neworder: Longword): Boolean; stdcall;
    procedure RD_ClearLayers(); stdcall;
    function RD_GetLayerCount(): Longword; stdcall;

    //�Ƿ�ʵʱ����
    procedure EnableRapidDraw(Enable: Boolean); stdcall;
    function RapidDrawEnabled(): Boolean; stdcall;
end;

//λ����ǩ����
type IPlaceBookmark = interface(IPersist)

    //������bookmark����ǰ��ת������Ϊbookmark��
    //����bookmark id��Ψһ��ʶ
    //����<0����ʧ��
    function AddBookmark(const text: PChar): Longint; stdcall;

    //ָ����Χ
    //����bookmark id��Ψһ��ʶ
    //����<0����ʧ��
    function AddBookmarkEx(var extent: WKSRect; const text: PChar): Longint; stdcall;

    //�޸�bookmark
    function ModifyBookmark(const id: Longint; var extent: WKSRect;
        const text: PChar): Boolean; stdcall;

    //ͨ��id�õ�bookmark
    function GetBookmarkByID(const id: Longint; out extent: WKSRect;
        out ppText: IAnsiString): Boolean; stdcall;

    //index <--> bookmark id
    //����<0����ʧ��
    function GetBookmarkIDByIndex(const index: Longint): Longint; stdcall;
    function GetBookmarkIndexByID(const id: Longint): Longint; stdcall;

    //�õ���ǰbookmark
    function GetCurrentBookmarkID(): Longint; stdcall;
    function GetCurrentBookmark(out extent: WKSRect; out ppText: IAnsiString): Boolean; stdcall;

    //����ʾ��Χ���óɵ�ǰ��bookmark
    function SetViewToCurrentBookmark(): Boolean; stdcall;

    //���õ�ǰbookmarkΪ��һ��bookmark
    //����bookmark id < 0 ��ʾ�Ѿ���ͷ
    function NextBookmark(): Longint; stdcall;

    //���õ�ǰbookmarkΪ��һ��bookmark
    //����bookmark id < 0 ��ʾ�Ѿ���ͷ
    function PreviousBookmark(): Longint; stdcall;

    //ɾ��ָ��id��bookmark
    function DeleteBookmark(const id: Longint): Boolean; stdcall;

    //ɾ������bookmark
    procedure ClearBookmarks(); stdcall;

    //����
    function GetBookmarkCount(): Longword; stdcall;

    //�Ƿ���ʾbookmark
    procedure SetBookmarksVisible(const visible: TBookMarkVisible); stdcall;
    function GetBookmarksVisible(): TBookMarkVisible; stdcall;

    //���塢��״�͵�ɫ
    function SetBookmarkSymbol(const pTextSymbol: ITextSymbol): Boolean; stdcall;
    function GetBookmarkSymbol(out ppTextSymbol: ITextSymbol): Boolean; stdcall;
    procedure SetBookmarkStyle(const style: TBookMarkStyle); stdcall;
    function GetBookmarkStyle(): TBookMarkStyle; stdcall;
    procedure SetBookmarkColor(const noactive: COLORREF; const active: COLORREF); stdcall;
    procedure GetBookmarkColor(out noactive: COLORREF; out active: COLORREF); stdcall;

    procedure DisableActiveBookmarkShow(); stdcall;

    //�洢
    function SaveBookmarksTo(pStream: IStreamX): Longword; stdcall;
    function LoadBookmarksFrom(pStream: IStreamX): Longword; stdcall;
end;

implementation

end.
