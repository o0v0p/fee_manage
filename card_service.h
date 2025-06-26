#include "model.h"
int deletecard();
int updatecard(card* pCard, int nIndex);
void updateExpiredCards();
card* doCancel(const char* name, const char* code);