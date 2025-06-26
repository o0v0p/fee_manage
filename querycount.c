//file name : querycrd.c
// 查询统计
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "menu.h"
#include "card_file.h"
#include "card_service.h"
#include "querycount.h"
#include "global.h"
#include <string.h>

void query_count() {
    printf("\n**************查询统计**************\n");
    printf("\n1.  卡状态查询\n2.  营业额统计\n");
    printf("\n====================================\n");
    printf("\n请输入编号选择查询项目：");
    int choice;
    scanf_s("%d", &choice);
    switch (choice) {
    case 1: checkcardStd(); break;
    case 2: totalMoney(); break;
    default: printf("\n输入有误！\n"); 
        printf("\n====================================\n"); 
        break;
    }
    pause();
}

void checkcardStd()
{
    if (readcard() == FALSE)
        return;
    int num1 = 0, num2 = 0, num3 = 0;
    ipcardnode pCur = *getCardListHead();
    if (pCur != NULL)
    {
        pCur = pCur->next;
        while (pCur != NULL)
        {
            if (pCur->cardo.nStatus == 0 || pCur->cardo.nStatus == 1)
                num2++;
            if (pCur->cardo.nStatus == 2 || pCur->cardo.nStatus == 3)
                num3++;
            pCur = pCur->next;
            num1++;
        }
    }
    printf("\n*********************卡状态查询信息信息如下*********************\n");
    printf("\n截止目前：\n共有%d张卡完成注册！\n有%d张卡仍然在使用中!\n有%d张卡已经办理注销!\n", num1, num2, num3);
    printf("\n================================================================\n");
}


void totalMoney() {
    double totalUsed = 0.0, totalRecharge = 0.0;
    ipcardnode pCur = NULL;
    ipcardnode head = *getCardListHead();
    if (head != NULL) {
        pCur = head->next;
        while (pCur != NULL) {
            totalUsed += pCur->cardo.fTotalUse;
            totalRecharge += pCur->cardo.fTotalUse + pCur->cardo.fBalance;
            pCur = pCur->next;
        }
        printf("\n*********************营业额统计信息信息如下*********************\n");
        printf("\n营业以来: \n共有用户充值%.2lf元!\n使用额度共%.2lf元\n", totalRecharge, totalUsed);
        printf("\n================================================================\n");
    }
}