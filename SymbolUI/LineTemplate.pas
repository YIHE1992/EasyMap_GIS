unit LineTemplate;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls;

type
    TEVENTTemplateModified = procedure() of Object;

const N_BARSTARTX = Longint(10);//bar��x��λ��
const N_BARSTARTY = Longint(105);//bar��y��λ��
const N_RECTWIDTH = Longint(6);//rect�Ŀ��
const N_RECTHEIGHT = Longint(6);//rect�ĸ߶�
const N_MAXLEN = Longint(60);//bar��rect��������(ע�ⲻ����ĩβ��һ����Чrect)

type
  TFormLineTemplate = class(TForm)
    BTClearTempl: TButton;
    EditTemplFactor: TEdit;
    UpDown4: TUpDown;
    Label6: TLabel;
    Label5: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure BTClearTemplClick(Sender: TObject);
    procedure EditTemplFactorChange(Sender: TObject);
  private
    { Private declarations }

    m_DC: HDC;

    m_PattRight: Longint;//��Ч�����ұߵ�rect-��ߵ�rect��
                        //ע�⵱û����Чrectʱm_nPattRight �� -1

    m_arrayPatt: array[0..N_MAXLEN] of Longint;//�������ģ����rectֵ������
                                //0����ɫ����Ч
                                //1����ɫ��ʵ
                                //2����ɫ����

    m_MouseMoveMode: Longint;//�����ж�����ƶ�ʱ�Ķ�����
                        //0��ʲôҲ����
                        //1���϶�ʱ�ı���Ч����ģ�峤��m_nPattLen

    m_MustBeRebuilt: Boolean;//�Ѿ��ı���ģ��

    procedure DrawRect(const Pos: Longint; const style: Longint);
    function MousePos(const x: Longint): Longint; overload;//�ж����λ��(mousemove)
    function MousePos(const x: Longint; const y: Longint): Longint; overload;//�ж����λ��(mousedown)
    procedure RebuildPattStore();//�ṩģ�����
    procedure DrawSample();
    procedure DrawScaleBar();//���ϱ�����

  public
    { Public declarations }
    m_arrayPattStore: array of Longint;
    m_Modified: Boolean;
    m_InProgress: Boolean;
    ModifyEvent: TEVENTTemplateModified;

    procedure Add(const sector: Longword);
    procedure ClearPattStore();//���ģ������
    procedure RefreshTemplate();//����m_arrayPattStore����ģ�������
  end;


implementation

{$R *.DFM}

procedure TFormLineTemplate.FormCreate(Sender: TObject);
var
    i: Longint;
begin
    ModifyEvent := nil;
    m_DC := 0;
    m_PattRight := -1;
    for i := 0 to N_MAXLEN do begin
        m_arrayPatt[i] := 0;
    end;

    SetLength(m_arrayPattStore, 0);
    m_MustBeRebuilt := False;
    m_InProgress := False;
end;

procedure TFormLineTemplate.FormPaint(Sender: TObject);
var
    i: Longint;
begin
    MoveToEx(m_DC, N_BARSTARTX, N_BARSTARTY - 2, nil);
    LineTo(m_DC, N_BARSTARTX + N_MAXLEN*(N_RECTWIDTH) + 2, N_BARSTARTY - 2);
    MoveToEx(m_DC, N_BARSTARTX, N_BARSTARTY + N_RECTHEIGHT + 1, nil);
    LineTo(m_DC, N_BARSTARTX + N_MAXLEN*(N_RECTWIDTH) + 2, N_BARSTARTY + N_RECTHEIGHT + 1);

    MoveToEx(m_DC, N_BARSTARTX, N_BARSTARTY - 2, nil);
    LineTo(m_DC, N_BARSTARTX, N_BARSTARTY + N_RECTHEIGHT + 1);
    MoveToEx(m_DC, N_BARSTARTX + N_MAXLEN*(N_RECTWIDTH) + 2, N_BARSTARTY - 2, nil);
    LineTo(m_DC, N_BARSTARTX + N_MAXLEN*(N_RECTWIDTH) + 2, N_BARSTARTY + N_RECTHEIGHT + 1);

    Self.DrawScaleBar();

    for i := 0 to N_MAXLEN-1 do begin
        Self.DrawRect(i, m_arrayPatt[i]);
    end;

    Self.DrawRect(m_PattRight + 1, 3);

    Self.DrawSample();

end;

procedure TFormLineTemplate.FormShow(Sender: TObject);
begin
    m_DC := GetDC(Self.Handle);
    m_Modified := False;    
end;

procedure TFormLineTemplate.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
    Action := caHide;

    ReleaseDC(Self.Handle, m_DC);
end;

procedure TFormLineTemplate.FormMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
    nClickPos, i: Longint;
begin
    if (mbLeft <> Button) then Exit;

    nClickPos := MousePos(X, Y);
    if (nClickPos >= 0) then begin
        if (nClickPos <= m_PattRight) then begin
            //������Чrect�иı�ģ��ͼ��
            if (m_arrayPatt[nClickPos] = 1) then begin
                m_arrayPatt[nClickPos] := 2;
                Self.DrawRect(nClickPos, 2);
            end
            else begin
                m_arrayPatt[nClickPos] := 1;
                Self.DrawRect(nClickPos, 1);
            end;
            m_MustBeRebuilt := True;
        end
        else if (nClickPos > m_PattRight + 1) then begin
            //������Чrect�ϣ�ֱ�Ӹı�ģ�峤��
            DrawRect(nClickPos, 3);
            for i := m_PattRight + 1 to nClickPos-1 do begin
                m_arrayPatt[i] := 2;
                Self.DrawRect(i, 2);
            end;

            m_PattRight := nClickPos - 1;
            m_MustBeRebuilt := True;
        end
        else begin
            //������ĩ�˵�rect
            m_MouseMoveMode := 1;//��ʼ�ı�m_nPattRight
        end;
    end;         
end;

procedure TFormLineTemplate.FormMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
    if (mbLeft <> Button) then Exit;

    if (m_MouseMoveMode = 1) then begin
        //����m_nPattRight�϶�
        m_MouseMoveMode := 0;
    end;

    if (m_MustBeRebuilt) then begin
        Self.RebuildPattStore();
        m_MustBeRebuilt := False;
        m_Modified := True;
    end;
end;

procedure TFormLineTemplate.FormMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
var
    nPattBarPos, i: Longint;
begin
    nPattBarPos := Self.MousePos(X);
    if ((m_MouseMoveMode = 1) and (nPattBarPos >= 0)) then begin
        //ע��nPattBarPos���ڵ�rect����Ч��
        if (nPattBarPos < m_PattRight + 1) then begin
            //�����϶���nPattBarPos�ұ߲���ɫrect
            for i := nPattBarPos to m_PattRight do begin
                m_arrayPatt[i] := 0;
                Self.DrawRect(i, 0);
            end;

            Self.DrawRect(m_PattRight + 1, 0);//ԭ�ȵ�ĩ����ɫrectҪ���ɵ�
            m_PattRight := nPattBarPos - 1;//�����趨����
            DrawRect(nPattBarPos, 3);//����ĩ����ɫrect

            m_MustBeRebuilt := True;

        end
        else if (nPattBarPos > m_PattRight + 1) then begin
            //�����϶���nPattBarPos��߲���ɫrect
            for i := m_PattRight+1 to nPattBarPos-1 do begin
                m_arrayPatt[i] := 2;
                DrawRect(i, 2);
            end;

            m_PattRight := nPattBarPos - 1;//�����趨����,ע��������ڵ�rect����Ч��
            DrawRect(nPattBarPos, 3);//����ĩ����ɫrect

            m_MustBeRebuilt := True;
        end;
    end;
end;

//********************************************
//    ��rect bar�ϻ�����rect
//    ������
//        *pDC��dialog���豸����
//        Pos�����ڵڼ���λ�ã���0��ʼ
//        style��    1 �� mark��2 �� gap��
//               3 �� end��other �� gray
//
//********************************************
procedure TFormLineTemplate.DrawRect(const Pos: Longint; const style: Longint);
var
    brush: HBRUSH;
    rect: TRect;
begin
    rect.top := N_BARSTARTY;
    rect.bottom := rect.top + N_RECTHEIGHT;
    rect.left := N_BARSTARTX + N_RECTWIDTH*Pos + 2;
    rect.right := N_BARSTARTX + N_RECTWIDTH*Pos + N_RECTWIDTH;
    Rectangle(m_DC, rect.left, rect.top, rect.right, rect.bottom);
    
    if (style = 1) then begin
        //ʵ��
        brush := CreateSolidBrush(clBlack);
    end
    else if (style = 2) then begin
        //����
        brush := CreateSolidBrush(clWhite);
    end
    else if (style = 3) then begin
        //����ģ���ĩβ����Ч
        brush := CreateSolidBrush(clBlue);
    end
    else begin
        //��Ч
        brush := CreateSolidBrush(clGray);
    end;

    FillRect(m_DC, rect, brush);
    DeleteObject(brush);
end;

//********************************************
//  �ж����x���ڵڼ���rect�ϣ�����y��
//    ��Ҫ��������϶�ʱ���ж�
//  ������
//        x,����������xֵ
//
//    ����ֵ��
//        >=0����ȷ��rect��
//        -1����ǰ���û����rect bar��
//
//********************************************
function TFormLineTemplate.MousePos(const x: Longint): Longint;
var
    nPos: Longint;
begin
    nPos := (x - N_BARSTARTX) div N_RECTWIDTH;
    if ((nPos >= 0) and (nPos < N_MAXLEN)) then begin
        Result := nPos;
    end
    else begin
        Result := -1;
    end;   
end;


//********************************************
//  �ж�����Ƿ���rect bar�ϣ��ڵڼ���rect�ϣ�
//    ��Ҫ���������ʱ���ж�
//  ������
//        x, y���������
//
//    ����ֵ��
//        >=0����ȷ��rect��
//        -1����ǰ���û����rect bar��
//
//********************************************
function TFormLineTemplate.MousePos(const x: Longint; const y: Longint): Longint;//�ж����λ��(mousedown)
var
    nPos: Longint;
begin
    nPos := (x - N_BARSTARTX) div N_RECTWIDTH;
    if ((nPos >= 0) and (nPos < N_MAXLEN) and (y >= N_BARSTARTY)
        and (y <= N_BARSTARTY + N_RECTHEIGHT)) then begin
        Result := nPos;
    end
    else begin
        Result := -1;
    end;
end;

//********************************************
//    ����m_arrayPatt���������¹���m_arrayPattStore
//
//********************************************
procedure TFormLineTemplate.RebuildPattStore();//�ṩģ�����
var
    i: Longint;
    PattSolid: Boolean;
begin
    SetLength(m_arrayPattStore, 0);
    if (m_PattRight < 0) then begin//û��ʵ��Ԫ�أ����ؿ�����
        Self.DrawSample();
        if (Assigned(ModifyEvent) and not m_InProgress) then begin
            ModifyEvent();
        end;
        Exit;
    end;

    SetLength(m_arrayPattStore, 1);
    m_arrayPattStore[0] := 0;
    PattSolid := True;
    if (m_arrayPatt[0] = 2) then begin
        //�����һ��Ԫ������ģ�������storeͷ������һ��ʵ��Ԫ�أ���ֵΪ0
        SetLength(m_arrayPattStore, Length(m_arrayPattStore) + 1);
        m_arrayPattStore[1] := 0;
        PattSolid := False;
    end;

    for i := 0 to m_PattRight do begin
        if (PattSolid) then begin
            //��һ����ʵ��
            if (m_arrayPatt[i] = 1) then begin
                //�����һ��Ҳ��ʵ��
                //store�����Ԫ����ֵ��1
                Inc(m_arrayPattStore[Length(m_arrayPattStore)-1]);
            end
            else begin
                //�����һ�������
                SetLength(m_arrayPattStore, Length(m_arrayPattStore)+1);
                m_arrayPattStore[Length(m_arrayPattStore)-1] := 1;
                PattSolid := False;
            end;
        end
        else begin
            //��һ�������
            if (m_arrayPatt[i] = 2) then begin
                //�����һ��Ҳ�����
                //store�����Ԫ����ֵ��1
                Inc(m_arrayPattStore[Length(m_arrayPattStore)-1]);
            end
            else begin
                //�����һ����ʵ��
                SetLength(m_arrayPattStore, Length(m_arrayPattStore)+1);
                m_arrayPattStore[Length(m_arrayPattStore)-1] := 1;
                PattSolid := True;
            end;
        end;
    end;

(*
    if (Length(m_arrayPattStore) mod 2 <> 0) then begin
        SetLength(m_arrayPattStore, Length(m_arrayPattStore)+1);
        m_arrayPattStore[Length(m_arrayPattStore)-1] := 0;
    end;
*)
    Self.DrawSample();

    if (Assigned(ModifyEvent) and not m_InProgress) then begin
        ModifyEvent();
    end;
end;

//********************************************
//    ���ϱ����ߣ�λ��������rect bar
//
//********************************************
procedure TFormLineTemplate.DrawScaleBar();
var
    i: Longint;
begin
    for i := 0 to N_MAXLEN do begin
        MoveToEx(m_DC, N_BARSTARTX + (i * N_RECTWIDTH), N_BARSTARTY - 6, nil);
        if ((i mod 5) = 0) then begin
            LineTo(m_DC, N_BARSTARTX + (i * N_RECTWIDTH), N_BARSTARTY - 13);
        end
        else begin
            LineTo(m_DC, N_BARSTARTX + (i * N_RECTWIDTH), N_BARSTARTY - 10);
        end;
    end;
end;

//********************************************
//    ���ģ�������
//
//********************************************
procedure TFormLineTemplate.ClearPattStore();
var
    i: Longint;
begin
    for i := 0 to N_MAXLEN - 1 do begin
        m_arrayPatt[i] := 0;
        Self.DrawRect(i, 0);
    end;

    m_PattRight := -1;

    Self.DrawRect(0, 3);

    Self.RebuildPattStore();
end;

//********************************************
//    ���ݵ�ǰm_arrayPattStore�����ݻ����߷���ʾ��
//
//********************************************
procedure TFormLineTemplate.DrawSample();
var
    rect: TRect;
    brush, oldbrush: HBRUSH;
    i, j, t, DotCount, DotLeft: Longint;
    solid: Boolean;
begin
    rect.top := 60;
    rect.left := 15;
    rect.bottom := 70;
    rect.right := 365;

    brush := CreateSolidBrush(RGB(240, 240, 255));
    FillRect(m_DC, rect, brush);
    DeleteObject(brush);

    brush := CreateSolidBrush(RGB(0, 0, 0));
    oldbrush := SelectObject(m_DC, brush);

    DotCount := 0;
    for t := 0 to 100 do begin
        solid := True;
        for i := 0 to Length(m_arrayPattStore)-1 do begin
            for j := 0 to m_arrayPattStore[i]-1 do begin
                if (solid) then begin
                    DotLeft := 20 + DotCount*3;
                    Rectangle(m_DC, DotLeft, 63, DotLeft + 3, 67);
                end;

                Inc(DotCount);
                if (DotCount > 110) then begin
                    SelectObject(m_DC, oldbrush);
                    DeleteObject(brush);
                    Exit;
                end;
            end;

            solid := not solid;
        end;
    end;

    SelectObject(m_DC, oldbrush);
    DeleteObject(brush);
end;

procedure TFormLineTemplate.RefreshTemplate();
var
    i, j, n, count: Longint;
    solid: Boolean;
begin
    for i := 0 to N_MAXLEN - 1 do begin
        m_arrayPatt[i] := 0;
        Self.DrawRect(i, 0);
    end;

    m_PattRight := -1;

    n := 0;
    solid := True;
    count := Length(m_arrayPattStore);
    for i := 0 to count - 1 do begin
        for j := 0 to m_arrayPattStore[i] - 1 do begin
            if (N_MAXLEN - 1 < n) then begin
                m_PattRight := n;
                Self.FormPaint(Self);
                Exit;
            end;

            if (solid) then begin
                m_arrayPatt[n] := 1;
            end
            else begin
                m_arrayPatt[n] := 2;
            end;
            Inc(n);
        end;

        solid := not solid;
    end;

    m_PattRight := n-1;
    Self.FormPaint(Self);
end;

procedure TFormLineTemplate.Add(const sector: Longword);
begin
    SetLength(m_arrayPattStore, Length(m_arrayPattStore)+1);
    m_arrayPattStore[Length(m_arrayPattStore)-1] := sector;
end;

procedure TFormLineTemplate.BTClearTemplClick(Sender: TObject);
begin
    Self.ClearPattStore();
    m_Modified := True;
end;

procedure TFormLineTemplate.EditTemplFactorChange(Sender: TObject);
begin
    if (Assigned(ModifyEvent) and not m_InProgress) then begin
        ModifyEvent();
    end;
end;

end.
