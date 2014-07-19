//˵�������ļ�����ĺ������ں����ڲ���̬�������壬�����������Զ�ɾ��
//      ���ļ������еĺ�����û��Ӧ��ȫ�ֱ����Ĵ���
#ifndef LDY_INTERFACE_H__
#define LDY_INTERFACE_H__
#include "LfcString.h"
#include "LfcTempl.h"

//Ȩ�޲�������
#define AUTH_AUTO			-1
#define AUTH_NONE					0		//û��Ȩ��
#define	AUTH_READ					1		//ֻ��Ȩ��
#define	AUTH_FULL					2		//��дȨ��

#define NULL_RIGHT  2  //Ȩ��û����ʱȱʡֵ



#define ITEMSIZE(p) (sizeof(p)/sizeof(p[0]))

#define CALL_FORM(FORM) {FORM *pForm; \
	try {	pForm=new FORM(Application); } \
	catch (Exception &exception) \
  { \
  	Application->ShowException(&exception); \
    return; \
	} \
  pForm->ShowModal(); \
  delete pForm; }

//�ȼ�Ȩ�����ýӿڣ��ļ�ClassRight.*
void ClassRight(int nAuth=AUTH_FULL);
//ȡ�ò����ȼ���Ȩ��
int GetClassRight(int nClass,LPCSTR lpszMenu);
//ȡ�ò���Ա��Ȩ��
int GetOperatorRight(LPCSTR	lpszOper,LPCSTR lpszMenu);

//------------------------------------------------------------------
//�ɹ����ŵ�����ӿڣ��ļ�StockDepart.*
void StockDepart(int nAuth=AUTH_FULL);

//���ű�����ӿڣ��ļ�DicDepart.*
void DicDepart(int nAuth=AUTH_FULL);

//��λ������ӿڣ��ļ�DicUnits.*
void DicUnits(int nAuth=AUTH_FULL);

//�����ص�����ӿڣ��ļ�GoodsAddr.*
void GoodsAddr(int nAuth=AUTH_FULL);

//״̬������ӿڣ��ļ�DicStatus.*
void DicStatus(int nAuth=AUTH_FULL);

//�������ϵ��ӿڣ��ļ���Factory.*
void FactoryInput(LPCSTR lpszID,int nAuth=AUTH_FULL);
void FactoryInput(int nAuth=AUTH_FULL);

//�������ϲ�ѯ�ӿڣ��ļ���FactorySearch.*
void FactorySearch(int nAuth=AUTH_FULL);

//������Ա���ӿڣ��ļ�HandleMan.*
void HandleMan(int nAuth=AUTH_FULL);

//���Ź�˾������ӿڣ��ļ�DepartCorp.*
void DepartCorp(int nAuth=AUTH_FULL);

//�빺������ӿڣ��ļ���AskPurchase.*
void AskPurchase(int nAuth=AUTH_FULL);
void AskPurchase(LPCSTR lpszID,int nAuth=AUTH_FULL);
//ͳ��������ӿڣ��ļ���AskPurchase.*
void UnionPurchase(LPCSTR lpszID,int nAuth=AUTH_FULL);

//���ϱ�Ų�ѯ�ӿ�
void QueryMaterialCode(int nAuth=AUTH_FULL);

//�빺��ȷ�Ͻӿڣ��ļ�AskPurchaseAfm.*
void AskPurchaseAfm(int nAuth=AUTH_FULL);

//�빺��ָ�ɲɹ�����ӿڣ��ļ�AskPurchaseSetSH.*
void AskPurchaseSetSH(int nAuth=AUTH_FULL);

//�ɹ����쵵�ӿڣ��ļ�StockHandleman.*
void StockHandleman(int nAuth=AUTH_FULL);

//���ʽ������ӿڣ��ļ�DicPayment.*
void DicPayment(int nAuth=AUTH_FULL);

//�빺���б��ѯ�ӿڣ��ļ���AskPurchaseList.*
void AskPurchaseList(int nAuth=AUTH_FULL);

//�빺���̽ӿڣ��ļ���PurchaseRoad.*
void PurchaseRoad(int nAuth=AUTH_FULL);

//����������ӿڣ��ļ���BookFactory.*
void BookFactory(int nAuth=AUTH_FULL);
void BookFactory(LPCSTR lpszID,int nAuth=AUTH_FULL);

//������Ʊ̧ͷ������ӿڣ��ļ�InvoiceTitle.*
void InvoiceTitle(int nAuth=AUTH_FULL);

//�ջ����쵵�ӿڣ��ļ�RcptHandleman.*
void RcptHandleman(int nAuth=AUTH_FULL);

//�빺���᰸�ӿڣ��ļ�AskPurchaseEnd.*
void AskPurchaseEnd(int nAuth=AUTH_FULL);
void AskPurchaseEnd(LPCSTR lpszID,int nAuth=AUTH_FULL);

//�������б��ѯ�ӿڣ��ļ���BookFactoryList.*
void BookFactoryList(int nAuth=AUTH_FULL);

//�ɹ���¼��ӿڣ��ļ���MaterBookList.*
void MaterBookList(int nAuth=AUTH_FULL);

//��Լ�ɹ����ӿڣ��ļ���TreatStock.*
void TreatStock(int nAuth=AUTH_FULL);
void TreatStock(LPCSTR lpszID,int nAuth=AUTH_FULL);

//��Լ�ɹ��б�ӿڣ��ļ���TreatStockList.*
void TreatStockList(int nAuth=AUTH_FULL);

//��Լ�ɹ���¼��ӿڣ��ļ���TreatStockRecord.*
void TreatStockRecord(int nAuth=AUTH_FULL);

//�ѽӰ�����δ����ϸ�ӿڣ��ļ���APAffirmAndNotBook.*
void APAffirmAndNotBook(int nAuth=AUTH_FULL);

//�½Ӱ���ͳ�Ʊ���ӿڣ��ļ���AskPurchaseAfmList.*
void AskPurchaseAfmList(int nAuth=AUTH_FULL);

//��������ӡ�ӿڣ��ļ���BookMultiPrn.*
void BookMultiPrn(int nAuth=AUTH_FULL);

//ѯ�۵���ӡ�ӿڣ��ļ���SpeerMultiPrn.*
void SpeerMultiPrn(int nAuth=AUTH_FULL);

//��������4�λ��ܱ�ӿڣ��ļ���Book4Times.*
void Book4Times(int nAuth=AUTH_FULL);

//�Ѷ���δ������ϸ�ӿڣ��ļ���BookNotAcceptList.*
void BookNotAcceptList(int nAuth=AUTH_FULL);

//������δ�����ϸ�ӿڣ��ļ���AcceptNotPayList.*
void AcceptNotPayList(int nAuth=AUTH_FULL);

//�������δ��ɰ���ͳ�Ʊ�ӿڣ��ļ���SumHiggleNotEnd.*
void SumHiggleNotEnd(int nAuth=AUTH_FULL);

//�ʺ�����δ��ɰ���ͳ�Ʊ�ӿڣ��ļ���SumDecideNotEnd.*
void SumDecideNotEnd(int nAuth=AUTH_FULL);

//��������δ��ɰ���ͳ�Ʊ�ӿڣ��ļ���SumBookNotEnd.*
void SumBookNotEnd(int nAuth=AUTH_FULL);

//����δ��������ͳ�Ʊ�ӿڣ��ļ���SumBookNotAcpt.*
void SumBookNotAcpt(int nAuth=AUTH_FULL);

//��Լ�ɹ�ȷ�Ͻӿڣ��ļ�TreatStockAfm.*
//������nType=0 ȷ�ϣ�nType=1 ���ȷ��
void TreatStockAfm(int nType,int nAuth=AUTH_FULL);

//ͳ�������嵥�ӿڣ��ļ���PrnUnitTreatList
void PrnUnitTreatList(int nAuth=AUTH_FULL);

//ͳ�������б�ӿڣ��ļ���UnitTreatList.*
void UnitTreatList(int nAuth=AUTH_FULL);

//���ż�����������ϱ�Ŷ��ձ�ӿڣ��ļ���MaterHXCompare.*
void MaterHXCompare(int nAuth=AUTH_FULL);

//ԭ�ϲɹ���¼��ӿڣ��ļ���StuffBookList.*
void StuffBookList(int nAuth=AUTH_FULL);
void StuffBookList(LPCSTR lpszID,int nAuth=AUTH_FULL);

//������ʽ������ӿڣ��ļ�DicDelivery.*
void DicDelivery(int nAuth=AUTH_FULL);

//��Ʊ˰�ʵ�����ӿڣ��ļ�DicInvoice.*
void DicInvoice(int nAuth=AUTH_FULL);

//ԭ���빺���б��ѯ�ӿڣ��ļ���RawPurchaseList.*
void RawPurchaseList(int nAuth=AUTH_FULL);

//ԭ�϶������б��ѯ�ӿڣ��ļ���RawBookFactoryList.*
void RawBookFactoryList(int nAuth=AUTH_FULL);

//�빺������ҳ�Ž���,�ļ���ApFilePage.*
void ApFilePage(int nAuth=AUTH_FULL);

//�빺���鵵�б�,�ļ���ApFPList.*
void ApFPList(int nAuth=AUTH_FULL);

//�����볧�̶���ͳ��,�ļ���SumMaterialnoFactory.*
void SumMnFact(int nAuth=AUTH_FULL);

//�������չʾ,�ļ���MaterialPicEdit.*
void MaterialPicture(int nAuth=AUTH_FULL);

//���ϱ��ԭ�� �ļ���F_MaterialRem
void MaterialRem(int nAuth=AUTH_FULL);

//�ɹ���¼ģ�������ӿڣ��ļ���MaterBookList2.*
void MaterBookList2(int nAuth=AUTH_FULL);

///bg
void Accept(int nAuth=AUTH_FULL);
void Do(int nAuth=AUTH_FULL);
void Finance(int nAuth=AUTH_FULL);
void Calc(int nAuth=AUTH_FULL);
void Assignment(int nAuth=AUTH_FULL);
void DicDicCurrency(int nAuth=AUTH_FULL);
void DicTargetCountry(int nAuth=AUTH_FULL);
void DicStatus1(int nAuth=AUTH_FULL);
void DicBargain(int nAuth=AUTH_FULL);
void DicExcharge(int nAuth=AUTH_FULL);
void DicZhengMian(int nAuth=AUTH_FULL);
void DicTrade(int nAuth=AUTH_FULL);
void DicTransport(int nAuth=AUTH_FULL);
void DicPack(int nAuth=AUTH_FULL);
void DicBoatNo(int nAuth=AUTH_FULL);
void DicBoatOrder(int nAuth=AUTH_FULL);
void DicMerchandise(int nAuth=AUTH_FULL);
void DicClient(int nAuth=AUTH_FULL);
void DicShipAgent1(int nAuth=AUTH_FULL);
void CostAccounting(int nAuth=AUTH_FULL);
void DicSH(int nAuth=AUTH_FULL);
void DicOperUnit(int nAuth=AUTH_FULL);
void DicCharge(int nAuth=AUTH_FULL);
void Tailer(int nAuth=AUTH_FULL);
void Receivables(int nAuth=AUTH_FULL);


#endif
