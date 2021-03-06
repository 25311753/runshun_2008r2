//---------------------------------------------------------------------------

#ifndef AskPurchaseAfmH
#define AskPurchaseAfmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TAskPurchaseAfmForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *txtTitle;
	TListView *lstView;
	TLabel *Label2;
	TEdit *edtID;
	TButton *btnRefresh;
	TButton *btnAfm;
	TButton *btnDeafm;
	TButton *btnExit;
	TButton *btnDetail;
	TButton *btnPrint;
	void __fastcall btnExitClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall lstViewSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall lstViewClick(TObject *Sender);
	void __fastcall btnAfmClick(TObject *Sender);
	void __fastcall btnDeafmClick(TObject *Sender);
	void __fastcall btnDetailClick(TObject *Sender);
	void __fastcall edtIDKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall btnRefreshClick(TObject *Sender);
	void __fastcall btnPrintClick(TObject *Sender);
private:	// User declarations
	void ResetCtrl();
	void RefreshData();

public:		// User declarations
	int m_nAuthor;

	__fastcall TAskPurchaseAfmForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAskPurchaseAfmForm *AskPurchaseAfmForm;
//---------------------------------------------------------------------------
#endif
