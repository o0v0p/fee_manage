#pragma once// 文件名：model.h
// 内容：定义程序中数据结构类型

#include <stdio.h>
#include <time.h>

//卡信息
typedef struct Card {
	char aName[19];		//卡号，长度为1~18个字符
	char aPwd[9];		//密码，长度为1~8个字符
	int nUseCount;		//使用次数
	int nStatus;		//卡状态  (0-未上机；1-正在上机；2-已注销；3-失效)
	int nDel; 			//删除标志  (0-未删除；1-已删除)
	time_t tStart;		//开卡时间,初始默认为添加卡的时间
	time_t tEnd;		//截止时间,初始默认为添加卡的时间
	time_t tLast;		//最后使用时间,初始默认为添加卡的时间
	double fTotalUse;	//累计消费
	double fBalance;		//余额
	double everymoney[100];  // 每次上机消费
	char everytime1[100][20];  //记录上机时间
	char everytime2[100][20];   //记录下机时间
	char everytime3[100][20];  //记录上机时间(仅年月日)
	char everytime4[100][20];   //记录下机时间(仅年月日)
}card;

//计费信息
typedef struct Fee {
	char aName[19];		//卡号，长度为1~18个字符
	double fAmount;		//消费
	time_t tStart;		//上机时间
	time_t tEnd;		//下机时间
	int nStatus;		//卡状态  (0-未结算；1-已结算)
	int nDel; 			//删除标志  (0-未删除；1-已删除)
}Billing;

//计费链表
typedef struct Billnode {
	Billing bill;
	struct Billnode* next;
}billnode, * ipbillnode;

//上机信息
typedef struct logonInfo {
	char aName[19];		//上机卡号
	double fBalance;	//上机时卡余额
	time_t tlogon;		//上机时间
}logoninfo;

//下机信息
typedef struct logoutInfo {
	char aName[19];			//卡号
	double fUse;	        //消费
	double fBalance;	    //余额
	time_t tlogon;		//上机时间
	time_t tlogout;		//下机时间
}logoutinfo;

//计费标准信息
typedef struct Standard {
	double charge;		//每个计费单元的收费
	int Starttime;		//开始时间
	int Endtime;		//结束时间
	int ratetype;		//收费类别(0-普通；1-包夜；2-包天)
	int unit;			//最小计费单元(分钟)
	int del; 			//删除标志  (0-未删除；1-已删除)
}standard;

//充值退费信息
typedef struct Refee {
	char aName[19];		//卡号，长度为1~18个字符
	double money;		//费用
	time_t feetime;		//操作时间
	int operation;		//操作类别  (0-充值；1-退费)
	int nDel; 			//删除标志  (0-未删除；1-已删除)
}refee;

//管理员信息
typedef struct Manager {
	char name[19];		//用户名，长度为1~18个字符
	char Pwd[9];		//密码，长度为1~8个字符
	int privilge;		//权限等级
	int nDel; 			//删除标志  (0-未删除；1-已删除)
}manager;

//卡片链表
typedef struct Cardnode {
	card cardo;
	struct Cardnode* next;
}cardnode, * ipcardnode;