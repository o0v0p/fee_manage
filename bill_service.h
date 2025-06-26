#include "model.h"
void logOn();
void logOut();
int savebilling(Billing* bill);
void Billlist();
void releaseBill();
ipbillnode querybill(char* aName, int* index);
int getBilling();
ipbillnode* getBillListHead();
logoutinfo* doSettleInfo(char* aNum, char* aCode, double* fee);
int updatefile(card* tocard);
card* doLogonInfo(char* aNum, char* aCode);
card* doLogon(char* aName, char* aCode);
card* doSettle(char* aNum, char* aCode, int* Indexc,double* fee);