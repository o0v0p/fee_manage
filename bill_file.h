int getbillcount();
int readBilling(Billing* pBilling);
int updatebill(Billing* pBilling, int nIndex);
Billing* checkBilling(char* aNum, int* pIndex,double* fee);