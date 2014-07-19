object InvoiceTitleForm: TInvoiceTitleForm
  Left = 258
  Top = 114
  Width = 747
  Height = 539
  Caption = #38283#31435#30332#31080#25260#38957#27284#36664#20837
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 256
    Top = 16
    Width = 297
    Height = 32
    Caption = #38283#31435#30332#31080#25260#38957#27284#36664#20837
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = #27161#26999#39636
    Font.Style = [fsBold]
    ParentFont = False
  end
  object GroupBox1: TGroupBox
    Left = 24
    Top = 64
    Width = 689
    Height = 441
    TabOrder = 0
    object msgState: TLabel
      Left = 24
      Top = 404
      Width = 377
      Height = 13
      AutoSize = False
      Caption = #24037#20316#29376#24907#65306
    end
    object ListView1: TListView
      Left = 24
      Top = 32
      Width = 433
      Height = 361
      Columns = <
        item
          Caption = #32232#34399
          Width = 80
        end
        item
          Caption = #21517#31281
          Width = 200
        end
        item
          Caption = #37096#38272#20844#21496#21029
          Width = 80
        end>
      GridLines = True
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
      OnClick = ListView1Click
    end
  end
  object Panel1: TPanel
    Left = 496
    Top = 96
    Width = 193
    Height = 185
    TabOrder = 1
    object Label2: TLabel
      Left = 16
      Top = 12
      Width = 36
      Height = 13
      Caption = #32232#34399#65306
    end
    object Label3: TLabel
      Left = 16
      Top = 60
      Width = 36
      Height = 13
      Caption = #21517#31281#65306
    end
    object Label1: TLabel
      Left = 16
      Top = 108
      Width = 72
      Height = 13
      Caption = #37096#38272#20844#21496#21029#65306
    end
    object edtID: TEdit
      Left = 16
      Top = 28
      Width = 161
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 0
      Text = 'EDTID'
    end
    object edtName: TEdit
      Left = 16
      Top = 76
      Width = 161
      Height = 21
      MaxLength = 30
      TabOrder = 1
      Text = 'edtName'
    end
    object btnOK0: TButton
      Left = 32
      Top = 152
      Width = 57
      Height = 25
      Caption = #30906#23450
      TabOrder = 3
      OnClick = btnOK0Click
    end
    object btnCancel0: TButton
      Left = 104
      Top = 152
      Width = 57
      Height = 25
      Cancel = True
      Caption = #21462#28040
      TabOrder = 4
      OnClick = btnCancel0Click
    end
    object edtCorp: TEdit
      Left = 16
      Top = 124
      Width = 161
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 2
      Text = 'EDTNAME'
    end
  end
  object btnAddNew: TButton
    Left = 512
    Top = 296
    Width = 161
    Height = 25
    Caption = #26032'    '#22686
    TabOrder = 2
    OnClick = btnAddNewClick
  end
  object btnEdit: TButton
    Left = 512
    Top = 328
    Width = 161
    Height = 25
    Caption = #20462'    '#25913
    TabOrder = 3
    OnClick = btnEditClick
  end
  object btnRefresh: TButton
    Left = 512
    Top = 392
    Width = 161
    Height = 25
    Caption = #21047'    '#26032
    TabOrder = 4
    OnClick = btnRefreshClick
  end
  object btnExit: TButton
    Left = 512
    Top = 456
    Width = 161
    Height = 25
    Caption = #36864'    '#20986
    TabOrder = 5
    OnClick = btnExitClick
  end
  object btnDelete: TButton
    Left = 512
    Top = 360
    Width = 161
    Height = 25
    Caption = #21034'    '#38500
    TabOrder = 6
    OnClick = btnDeleteClick
  end
  object btnPrint: TButton
    Left = 512
    Top = 424
    Width = 161
    Height = 25
    Caption = #21015'    '#21360
    TabOrder = 7
    OnClick = btnPrintClick
  end
end
