object NewSymbol: TNewSymbol
  Left = 211
  Top = 107
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = '��������'
  ClientHeight = 112
  ClientWidth = 328
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '����'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object BitBtn4: TBitBtn
    Left = 232
    Top = 80
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'ȡ��'
    TabOrder = 0
    OnClick = BitBtn4Click
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 328
    Height = 73
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 8
      Top = 8
      Width = 97
      Height = 49
      Caption = '�����'
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 112
      Top = 8
      Width = 97
      Height = 49
      Caption = '�߷���'
      TabOrder = 1
      OnClick = BitBtn2Click
    end
    object BitBtn3: TBitBtn
      Left = 216
      Top = 8
      Width = 97
      Height = 49
      Caption = '�����'
      TabOrder = 2
      OnClick = BitBtn3Click
    end
  end
end
