#include "model.h"
void recharge();
void refund();
void query_count();
double fee_count(time_t logtime);
card* domoney_refund(char* aName, char* aPwd, double* money);
card* domoney_recharge(char* aName, char* aPwd, double* money);
int savefee(refee* fee);
int updatefee(refee* feeinfo);
double fee_count(time_t logtime);
