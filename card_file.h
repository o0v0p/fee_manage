#include "model.h"
void Cardlist();
void releaseCardlist();
int savecard(card pcard,char* ppath);
void querycard();
ipcardnode search(char* aName);
void addCardlist(card cardo);
int getcardcount();
//int getCard();
ipcardnode* getCardListHead();
card* lookrepet(char* aName,int* index);
int readcard();
void queryInfo();