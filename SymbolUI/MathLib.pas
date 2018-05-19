unit MathLib;

interface

uses
    Math, WKSStructs;

//------------------------------------------------------------------------------
type TCROSSRESULTTYPE = Longint;             //�󽻵㺯���ķ���ֵ����
//�󽻵㺯���ķ���ֵ����
const R_2LINES_CROSS = 1;               //�����߶��ཻ
const R_2LINES_NOCROSS = -1;            //�����߶��޽��㣨�������ӳ����ϣ�
const R_2LINES_PARALLEL = 0;            //�����߶�ƽ��
const R_2LINES_COVER = 2;               //�����߶��໥����
const R_2LINES_LINENOCROSS = -2;        //�����߶���һ��ֱ���ϣ����޽���

const R_CIRCLELINE_CROSS2 = 2;          //Բ���߶��ཻ����������
const R_CIRCLELINE_CROSS1 = 1;          //Բ���߶��ཻ��һ�����㣨��һ�����߶��ӳ����ϣ�
const R_CIRCLELINE_TANGENT = 3;         //Բ���߶����У�
const R_CIRCLELINE_NOINCROSS = -1;      //Բ���߶��޽��㣨���㶼���ӳ����ϣ�
const R_CIRCLELINE_NOCROSS = 0;         //Բ���߶��޽��㣨���ӳ����϶�û�н��㣩

//�ж��󽻵㺯���ķ���ֵ���Ƿ��н���
function ISLINECROSS(r: TCROSSRESULTTYPE): Boolean;

//��ƽ����y���ֱ��(x = X)���߶εĽ���
function LineCrossY(
    x1, y1, x2, y2: Double;//�߶�
    X: Double;//ֱ��x = X
    var Y: Double//����y����
): TCROSSRESULTTYPE;

//��ƽ����x���ֱ��(y = Y)���߶εĽ���
function LineCrossX(
    x1, y1, x2, y2: Double;//�߶�
    Y: Double;//ֱ��y = Y
    var X: Double//����x����
): TCROSSRESULTTYPE;

//�������߶εĽ���
function LineCross(
    x1, y1, x2, y2: Double;//�߶�1
    x3, y3, x4, y4: Double; //�߶�2
    var x: Double;//����x����
    var y: Double//����y����
): TCROSSRESULTTYPE;

//------------------------------------------------------------------------------


//�Ƚ�����˫���ȸ������Ƿ����
function CompareDouble(a, b: Double): Boolean;

//��������
procedure DoQuickSort(var A: array of Double);

//ȥ���Ѿ����������е��ظ�Ԫ��
function UniqueSortedArray(var A: array of Double): Longint;

const PIE = 3.1415926536;

//------------------------------------------------------------------------------

//���Ƿ��ڶ������
//      1   �ڱ߽���
//      >1  ���ڲ�
//      0   ���ⲿ
//      <0  ����
function PointInPolygon(point: WKSPoint; var pointarray: array of WKSPoint;
    pointcount: Longint): Longint;

//�����Ƿ�������ཻ
function EnvelopeCrossPath(envelope: WKSRect; var pointarray: array of WKSPoint;
    pointcount: Longint): Boolean;

procedure RotatePI(var pnt: WKSPoint; var origin: WKSPoint; const angle: Double);
procedure RotateDegree(var pnt: WKSPoint; var origin: WKSPoint; const degree: Double);

implementation

function CompareDouble(a, b: Double): Boolean;
begin
    if (Abs(a - b) < 0.0001) then begin
        Result := True;
    end
    else begin
        Result := False;
    end;
end;

procedure DoQuickSort(var A: array of Double);
  procedure QuickSort(var A: array of Double; iLo, iHi: Longint);
  var
    Lo, Hi: Longint;
    Mid, T: Double;
  begin
    Lo := iLo;
    Hi := iHi;
    Mid := A[(Lo + Hi) div 2];
    repeat
      while A[Lo] < Mid do Inc(Lo);
      while A[Hi] > Mid do Dec(Hi);
      if Lo <= Hi then
      begin
        T := A[Lo];
        A[Lo] := A[Hi];
        A[Hi] := T;
        Inc(Lo);
        Dec(Hi);
      end;
    until Lo > Hi;
    if Hi > iLo then QuickSort(A, iLo, Hi);
    if Lo < iHi then QuickSort(A, Lo, iHi);
  end;

begin
  QuickSort(A, Low(A), High(A));
end;

function UniqueSortedArray(var A: array of Double): Longint;
var
    i, j, count: Longint;
    B: array of Double;
begin
    count := High(A) + 1;
    if (count <= 0) then begin
        Result := 0;
        Exit;
    end;

    SetLength(B, count);

    B[0] := A[0];
    j := 0;
    for i := 1 to count - 1 do begin
        if (CompareDouble(A[i], A[i - 1])) then begin
            //�ظ�
            Continue;
        end
        else begin
            Inc(j);
            B[j] := A[i];
        end;
    end;

    //������A���
    for i := 0 to j do begin
        A[i] := B[i];
    end;

    Result := j + 1;
end;



//�ж�x�Ƿ���a��b������
function InRegion(a, b, x: Double): Boolean;
var
    fmin, fmax: Double;
begin
    fmin := Min(a, b);
    fmax := Max(a, b);

    if ((x >= fmin) and (x <= fmax)) then begin
        Result := True;
    end
    else begin
        if (CompareDouble(x, a)) then begin
            Result := True;
        end
        else if (CompareDouble(x, b)) then begin
            Result := True;
        end
        else begin
            Result := False;
        end;
    end;

end;

//�жϵ��Ƿ��ھ��η�Χ��
function PointInEnvelope(x1, y1, x2, y2: Double;//���η�Χ
                         x, y: Double    //��
                         ): Boolean;
begin
    if (InRegion(x1, x2, x) and InRegion(y1, y2, y)) then begin
        Result := True;
    end
    else begin
        Result := False;
    end;
end;

//�ж��󽻵㺯���ķ���ֵ���Ƿ��н���
function ISLINECROSS(r: TCROSSRESULTTYPE): Boolean;
begin
    if (r > 0) then begin
        Result := True;
    end
    else begin
        Result := False;
    end;
end;

//��ƽ����y���ֱ��(x = X)���߶εĽ���
function LineCrossY(x1, y1, x2, y2: Double;//�߶�
                    X: Double;//ֱ��x = X
                    var Y: Double//����y����
                    ): TCROSSRESULTTYPE;
var
    k, b: Double;
begin
    //��������߶ε�б��
    if (x2 <> x1) then begin
        k := (y2 - y1) / (x2 - x1);
    end
    else begin
        //����ֱ��ƽ�У��ж��Ƿ��غ�
        if (x1 = X) then begin
            Result := R_2LINES_COVER;
            Y := y1;
        end
        else begin
            Result := R_2LINES_PARALLEL;
        end;
        Exit;
    end;
    //��������߶ε�b
    b := y1 - k * x1;
    //��x = X���뷽�̣��������������y
    Y := k * X + b;

    if (InRegion(x1, x2, X)) then begin
        //�����߶��ཻ
        Result := R_2LINES_CROSS;
    end
    else begin
        //�����߶��޽��㣨�������ӳ����ϣ�
        Result := R_2LINES_NOCROSS;
    end;
end;

//��ƽ����x���ֱ��(y = Y)���߶εĽ���
function LineCrossX(x1, y1, x2, y2: Double;//�߶�
                    Y: Double;//ֱ��y = Y
                    var X: Double//����x����
                    ): TCROSSRESULTTYPE;
var
    k, b: Double;
begin
    //��������߶ε�б��
    if (x2 <> x1) then begin
        k := (y2 - y1) / (x2 - x1);
    end
    else begin
        X := x1;
        //����ֱ����x�ᴹֱ
        if (InRegion(y1, y2, Y)) then begin
            Result := R_2LINES_CROSS;
        end
        else begin
            Result := R_2LINES_NOCROSS;
        end;
        Exit;
    end;
    //��������߶ε�b
    b := y1 - k * x1;
    //��y = Y���뷽�̣�������������x
    if (k <> 0) then begin
        X := (Y - b) / k;
    end
    else begin
        //����ֱ��ƽ�У��ж��Ƿ��غ�
        if (y1 = Y) then begin
            Result := R_2LINES_COVER;
            X := x1;
        end
        else begin
            Result := R_2LINES_PARALLEL;
        end;
        Exit;
    end;

    if (InRegion(y1, y2, Y)) then begin
        //�����߶��ཻ
        Result := R_2LINES_CROSS;
    end
    else begin
        //�����߶��޽��㣨�������ӳ����ϣ�
        Result := R_2LINES_NOCROSS;
    end;
end;

//�������߶εĽ���
function LineCross(x1, y1, x2, y2: Double;//�߶�1
                   x3, y3, x4, y4: Double; //�߶�2
                   var x: Double;//����x����
                   var y: Double//����y����
                   ): TCROSSRESULTTYPE;
var
    k1, k2, b1, b2: Double;
    bVertical1, bVertical2: Boolean;
    nCrossResult: Longint;
begin
    if (x2 <> x1) then begin
        //����߶�1��б��k1
        k1 := (y2 - y1) / (x2 - x1);
        //����߶�1��b1
        b1 := y1 - k1 * x1;
        bVertical1 := False;
    end
    else begin
        //�߶�1��ֱ��x��
        x := x1;
        bVertical1 := True;

        k1 := 0;b1 := 0;//����
    end;

    if (x3 <> x4) then begin
        //����߶�2��б��k2
        k2 := (y3 - y4) / (x3 - x4);
        //����߶�2��b2
        b2 := y3 - k2 * x3;
        bVertical2 := False;
    end
    else begin
        //�߶�2��ֱ��x��
        x := x3;
        bVertical2 := True;

        k2 := 0;b2 := 0;//����
    end;

    if ((not bVertical1) and (not bVertical2)) then begin
        //�ⷽ��
        if (k1 <> k2) then begin
            x := (b2 - b1) / (k1 - k2);
            y := k1 * x + b1;
        end
        else begin
            if (b1 = b2) then begin
                //�ж����߶��Ƿ��໥����
                if (not PointInEnvelope(x1, y1, x2, y2, x3, y3)) then begin
                    if (not PointInEnvelope(x1, y1, x2, y2, x4, y4)) then begin
                        if (not PointInEnvelope(x3, y3, x4, y4, x1, y1)) then begin
                            if (not PointInEnvelope(x3, y3, x4, y4, x2, y2)) then begin
                                //�޽���
                                Result := R_2LINES_LINENOCROSS;
                                Exit;
                            end;
                        end;
                    end;
                end;
                //�໥����
                Result := R_2LINES_COVER;
            end
            else begin
                //��ֱ��ƽ��
                Result := R_2LINES_PARALLEL;
            end;
            Exit;
        end;
    end
    else begin
        //������߶ζ���ֱ��x��
        if (bVertical1 and bVertical2) then begin
            if (x1 <> x3) then begin
                //��ֱ�߻���ƽ��
                Result := R_2LINES_PARALLEL;
                Exit;
            end
            else begin
                //�ж��Ƿ��໥����
                if (InRegion(y1, y2, y3)) then begin
                    Result := R_2LINES_COVER;
                    Exit;
                end;
                if (InRegion(y1, y2, y4)) then begin
                    Result := R_2LINES_COVER;
                    Exit;
                end;
                if (InRegion(y3, y4, y1)) then begin
                    Result := R_2LINES_COVER;
                    Exit;
                end;
                if (InRegion(y3, y4, y2)) then begin
                    Result := R_2LINES_COVER;
                    Exit;
                end;
                //�޽���
                Result := R_2LINES_LINENOCROSS;
                Exit;
            end;
        end;
        //
        if (bVertical1) then begin
            nCrossResult := LineCrossY(x3, y3,
                                       x4, y4,
                                       x,
                                       y);
            if (not ISLINECROSS(nCrossResult)) then begin
                //��ֱ��ƽ�У��޽���
                Result := nCrossResult;
                Exit;
            end;
        end
        else begin
            if (bVertical2) then begin
                nCrossResult := LineCrossY(x1, y1,
                                           x2, y2,
                                           x,
                                           y);
                if (not ISLINECROSS(nCrossResult)) then begin
                    //��ֱ��ƽ�У��޽���
                    Result := nCrossResult;
                    Exit;
                end;
            end;
        end;
    end;

    if (PointInEnvelope(x1, y1,
                     x2, y2,
                     x, y)) then begin
        if(PointInEnvelope(x3, y3,
                        x4, y4,
                        x, y)) then begin
            //�����߶��ཻ
            Result := R_2LINES_CROSS;
            Exit;
        end;
    end;

    //�����߶��޽��㣨�������ӳ����ϣ�
    Result := R_2LINES_NOCROSS;

end;

function PointInPolygon(point: WKSPoint; var pointarray: array of WKSPoint;
    pointcount: Longint): Longint;
var
    ptStart, ptEnd: WKSPoint;
    nDist, tmp: Double;
    i, nTempCross: Longint;
    a, b, c, d: Boolean;
begin
    Result := 0;
    ptStart := pointarray[0];

    for i := 0 to pointcount - 1 do begin
        ptEnd := pointarray[i];
        if (CompareDouble(ptStart.x, ptEnd.x)) then begin
            if (CompareDouble(ptStart.y, ptEnd.y)) then Continue;      
            if (CompareDouble(ptStart.x, point.x)
                and (((point.y <= ptStart.y) and (point.y >= ptEnd.y))
                or ((point.y >= ptStart.y) and (point.y <= ptEnd.y)))) then begin
                Result := 1;
                Exit;
            end;
        end
        else begin
            nTempCross := 2;
            a := ptStart.x <= point.x;
            b := ptEnd.x >= point.x;
            c := ptStart.x >= point.x;
            d := ptEnd.x <= point.x;
            if (((a and b) or (c and d))) then begin
                a := CompareDouble(ptStart.x, point.x);
                b := CompareDouble(ptEnd.x, point.x);
                if (a or b) then begin
                    nTempCross := 1;
                end;
                nDist := (point.y - ptStart.y) * (ptEnd.x - ptStart.x) -
                    (point.x - ptStart.x) * (ptEnd.y - ptStart.y);
                tmp := ((ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y) -
                    (ptEnd.x - ptStart.x ) * ( ptEnd.x - ptStart.x));
                if (0.0001 < Abs(tmp)) then begin
                    if (Abs(nDist * nDist / tmp) < 0.0001) then begin
                        Result := 1;   //on boundary
                        Exit;
                    end
                    else begin
                        if (nDist > 0.0) then begin
                            nTempCross := -nTempCross;
                        end;
                        Result := Result + nTempCross;
                    end;
                end
                else begin
                    if (nDist > 0.0) then begin
                        nTempCross := -nTempCross;
                    end;
                    Result := Result + nTempCross;
                end;
            end;
        end;
        ptStart := ptEnd;
    end;      
end;

function EnvelopeCrossLine(envelope: WKSRect; linefrom, lineto: WKSPoint): Boolean;
var
    crt: TCROSSRESULTTYPE;
    x, y: Double;
begin
    crt := LineCross(envelope.left, envelope.top, envelope.left, envelope.bottom,
        linefrom.x, linefrom.y, lineto.x, lineto.y,
        x, y);
    Result := ISLINECROSS(crt);
    if (Result) then Exit;

    crt := LineCross(envelope.left, envelope.bottom, envelope.right, envelope.bottom,
        linefrom.x, linefrom.y, lineto.x, lineto.y,
        x, y);
    Result := ISLINECROSS(crt);
    if (Result) then Exit;

    crt := LineCross(envelope.right, envelope.bottom, envelope.right, envelope.top,
        linefrom.x, linefrom.y, lineto.x, lineto.y,
        x, y);
    Result := ISLINECROSS(crt);
    if (Result) then Exit;

    crt := LineCross(envelope.right, envelope.top, envelope.left, envelope.top,
        linefrom.x, linefrom.y, lineto.x, lineto.y,
        x, y);
    Result := ISLINECROSS(crt);
end;

function EnvelopeCrossPath(envelope: WKSRect; var pointarray: array of WKSPoint;
    pointcount: Longint): Boolean;
var
    i: Longint;
begin
    Result := False;

    for i := 0 to pointcount - 2 do begin
        if (EnvelopeCrossLine(envelope, pointarray[i], pointarray[i+1])) then begin
            Result := True;
            Break;
        end;
    end;             
end;

procedure RotateDegree(var pnt: WKSPoint; var origin: WKSPoint; const degree: Double);
begin
    RotatePI(pnt, origin, (360-degree)*PI*2.0/360.0);
end;

procedure RotatePI(var pnt: WKSPoint; var origin: WKSPoint; const angle: Double);
var
     d: WKSPoint;
     x, y, cosx, siny: Double;
begin
     d.x := pnt.x - origin.x;
     d.y := pnt.y - origin.y;
     cosx := Cos(angle);
     siny := Sin(angle);
     x := d.x*cosx - d.y*siny;
     y := d.y*cosx + d.x*siny;
     pnt.x := origin.x + x;
     pnt.y := origin.y + y;
end;

end.
