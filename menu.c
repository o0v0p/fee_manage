//file name : menu.c
// 菜单

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "model.h"
#include "global.h"
#include "bill_file.h"
#include "bill_service.h"
#include "card_file.h"
#include "card_service.h"
#include "fee_count.h"
#include "tool.h"
#include "menu.h"
#include "querycount.h"

int cardcount = 0;
void outmenu() {										//输出菜单
	printf("\n**************菜单**************\n\n");
	printf("1. 添加卡\n");
	printf("2. 查询卡\n");
	printf("3. 上机\n");
	printf("4. 下机\n");
	printf("5. 充值\n");
	printf("6. 退费\n");
	printf("7. 注销卡\n");
	printf("8. 查询统计\n");
	printf("0. 退出系统\n\n");
	updateExpiredCards();
}

void addcard()
{
	card card0 = { 0 };
	int namesize = 0, pwdsize = 0;
	printf("\n**************添加卡**************\n\n");
	printf("请输入卡号 <长度为1~18>：");							//输入卡号
	scanf_s("%s", card0.aName, (unsigned int)sizeof(card0.aName));
	namesize = getsize(card0.aName);
	if (namesize > 18 || namesize < 1) {
		printf("\n卡号长度不符合要求！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	if (cardcount != 0) {
		int virtual = 0;
		card* position = lookrepet(card0.aName, &virtual);								//卡号不可重复
		if (position != NULL) {
			printf("\n该卡号已存在！\n");
			printf("\n==================================\n");
			pause();
			return;
		}
	}
	printf("请输入密码 <长度为1~8>：");								//输入密码
	scanf_s("%s", card0.aPwd, (unsigned int)sizeof(card0.aPwd));
	pwdsize = getsize(card0.aPwd);
	if (pwdsize > 8 || pwdsize < 1) {
		printf("\n密码长度不符合要求！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("请输入开卡金额 <RMB>：");								//输入金额
	if (scanf_s("%lf", &card0.fBalance) == 0) {
		printf("\n金额格式不正确！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	// 初始化时间字段
	card0.fTotalUse = 0;							//初始化
	card0.nDel = 0;
	card0.nStatus = 0;
	card0.nUseCount = 0;
	card0.tStart = card0.tLast = card0.tEnd = time(NULL);
	struct tm Tm = { 0 };
	localtime_s(&Tm, &card0.tStart);
	Tm.tm_year += 1;
	card0.tEnd = mktime(&Tm);
	// 初始化时间数组
	for (int i = 0; i < 100; i++) {
		memset(card0.everytime1[i], 0, 20);
		memset(card0.everytime2[i], 0, 20);
		memset(card0.everytime3[i], 0, 20);
		memset(card0.everytime4[i], 0, 20);
		card0.everymoney[i] = 0;
	}
	if (cardcount == 0) {										//录文件与链表
		FILE* fp;
		errno_t err = fopen_s(&fp, cardpath, "w+");
		if (err != 0 || fp == NULL)
			printf("文件打开失败！");
		if (fp != 0)
			fclose(fp);
		else {
			printf("文件录入出错！");
			exit(1);
		}
	}
	cardcount++;
	if (savecard(card0, cardpath))
	{
		//展示
		printf("\n*********************添加成功！新卡信息如下*********************\n\n");
		printf("%-21s %-14s %-11s %-10s\n", "卡号", "密码", "状态", "开卡金额");
		printf("%-20s  %-15s  %-10d  %-10.1f \n\n", card0.aName, card0.aPwd, card0.nDel, card0.fBalance);
		printf("================================================================\n");
		readcard();
		pause();
		return;
	}
	else
	{
		printf("添加失败!\n");
	}
}

int getsize(const char* input) {							//判断输入长度
	int size = 0;
	size = (int)strlen(input);
	return size;
}

void pause() {									//按Enter键继续
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	printf("按Enter键继续...");
	while ((ch = getchar()) != '\n' && ch != EOF);
}
