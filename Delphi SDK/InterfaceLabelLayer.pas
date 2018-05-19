unit InterfaceLabelLayer;

interface

uses
    InterfaceObj, InterfaceDisplayTransformation, InterfaceDisplay, InterfaceLayer,
    InterfaceTrackCancel, WKSStructs, Windows;

type ILabelLayer = interface(ILayer)
    //  ������SlimLayer����ShapeLayer
    function SetVectorLayer(const pLayer: ILayer): Boolean; stdcall;
    function GetVectorLayer(out ppLayer: ILayer): Boolean; stdcall;

    //  ��ע�ֶ�����
    function SetFieldIndex(const fieldindex: Longint): Boolean; stdcall;
    function GetFieldIndex(): Longint; stdcall;

    //  ע�Ƿ���
    function SetTextSymbol(const pTextSymbol: ITextSymbol): Boolean; stdcall;
    function GetTextSymbol(out ppTextSymbol: ITextSymbol): Boolean; stdcall;

    //  �ο�������
    function SetRefScale(const scale: Double): Boolean; stdcall;
    function GetRefScale(): Double; stdcall;

    //  ȡ�����ӷ�Χ�ڵı�ע����
    function GetLabelText(out ppTextPositions: IDoubleArray; out ppLabelTexts: IStringArray;
        const pDT: IDisplayTransformation; const pTrackCancel: ITrackCancel;
        var visibleextent: WKSRect): TDrawResult; stdcall;

    //  ��ջ������ݣ����ں�����ͼ��ͬ��
    procedure ClearCache(); stdcall;
end;

type ILabelLayerManager = interface(IPersist)
    //  ����һ��LabelLayer
    function AddLabelLayer(const pLabelLayer: ILabelLayer): Boolean; stdcall;
    function RemoveLabelLayer(const index: Longword): Boolean; stdcall;
    function RemoveLabelLayerEx(pLabelLayer: ILabelLayer): Boolean; stdcall;
    function GetLabelLayer(out ppLabelLayer: ILabelLayer; const index: Longword): Boolean; stdcall;
    function SetLabelLayerOrder(const pLabelLayer: ILabelLayer; const neworder: Longword): Boolean; stdcall;
    procedure ClearLabelLayers(); stdcall;
    function GetLabelLayerCount(): Longword; stdcall;

    //  �Ƿ����
    procedure SetTextAvoidable(const avoidable: Boolean); stdcall;
    function GetTextAvoidable(): Boolean; stdcall;

    //  ���Ʊ�ע
    function DrawLabels(const pDisplay: IDisplay; const pEnvelope: PWKSRect;
        const pTrackCancel: ITrackCancel): TDrawResult; stdcall;

    //  �Ƿ���ʾ�Զ���ע
    procedure EnableLabelDraw(Enable: Boolean); stdcall;
    function LabelDrawEnabled(): Boolean; stdcall;
end;

implementation

end.
