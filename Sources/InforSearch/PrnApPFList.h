//---------------------------------------------------------------------------

#ifndef PrnApPFListH
#define PrnApPFListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <QuickRpt.hpp>
#include <Qrctrls.hpp>
#include <QRCtrls.hpp>
//---------------------------------------------------------------------------
class TPrnApFPListForm : public TForm
{
__published:	// IDE-managed Components
        TQuickRep *PrnView;
        TQRBand *DetailBand1;
        TQRBand *PageHeaderBand1;
        TQRLabel *QRLabel1;
        TQRShape *QRShape1;
        TQRShape *QRShape2;
        TQRShape *QRShape3;
        TQRShape *QRShape4;
        TQRShape *QRShape5;
        TQRShape *QRShape6;
        TQRShape *QRShape7;
        TQRShape *QRShape8;
        TQRShape *QRShape9;
        TQRShape *QRShape10;
        TQRShape *QRShape11;
        TQRShape *QRShape12;
        TQRShape *QRShape13;
        TQRLabel *QRLabel2;
        TQRLabel *QRLabel3;
        TQRLabel *QRLabel4;
        TQRLabel *QRLabel5;
        TQRLabel *QRLabel6;
        TQRLabel *QRLabel7;
        TQRLabel *QRLabel8;
        TQRLabel *QRLabel9;
        TQRLabel *QRLabel10;
        TQRLabel *QRLabel11;
        TQRLabel *QRLabel12;
        TQRLabel *QRLabel13;
        TQRShape *QRShape14;
        TQRShape *QRShape15;
        TQRShape *QRShape16;
        TQRShape *QRShape17;
        TQRShape *QRShape18;
        TQRShape *QRShape19;
        TQRShape *QRShape20;
        TQRShape *QRShape21;
        TQRShape *QRShape22;
        TQRShape *QRShape23;
        TQRShape *QRShape24;
        TQRShape *QRShape25;
        TQRShape *QRShape26;
        TQRLabel *QRLabel14;
        TQRDBText *QRDBText1;
        TQRDBText *QRDBText2;
        TQRDBText *QRDBText3;
        TQRDBText *QRDBText4;
        TQRDBText *QRDBText5;
        TQRDBText *QRDBText6;
        TQRDBText *QRDBText7;
        TQRDBText *QRDBText8;
        TQRDBText *QRDBText9;
        TQRDBText *QRDBText10;
        TQRDBText *QRDBText11;
        TQRDBText *QRDBText12;
        TQRDBText *QRDBText13;
        TQRLabel *QRLabel15;
        TQRSysData *QRSysData1;
        TQRDBText *QRDBText14;
        TQRSysData *QRSysData2;
private:	// User declarations
public:		// User declarations
        __fastcall TPrnApFPListForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPrnApFPListForm *PrnApFPListForm;
//---------------------------------------------------------------------------
#endif
