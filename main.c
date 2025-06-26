//file name : main.c
// 计费管理系统主程序

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "model.h"
#include "global.h"
#include "bill_service.h"
#include "bill_file.h"
#include "card_file.h"
#include "card_service.h"
#include "fee_count.h"
#include "querycount.h"

void implement(int n);

int main() {                                    //主程序入口
    int choice = -1;
    char ch;
    Cardlist();
    Billlist();
    printf("\n欢迎来到计费管理系统!\n");
    do {
        choice = -1;
        outmenu();                                //输出菜单
        printf("请选择菜单项编号（0-8）: ");
        int check = scanf_s("%d", &choice);
        while ((ch = getchar()) != '\n' && ch != EOF);
        if (0 <= choice && choice <= 8 && check)
            implement(choice);                        //根据输入编号 执行 相应功能
        else {
            printf("==================================\n");
            printf("\n输入错误，请重新输入！\n");
        }
    } while (choice != 0);                        //循环菜单
    return 0;
}

void implement(int n) {
    switch (n) {
    case 1: addcard(); break;                //添加卡
    case 2: querycard(); break;                //查询卡
    case 3: logOn(); break;                    //上机
    case 4: logOut(); break;                //下机
    case 5: recharge(); break;                //充值
    case 6: refund(); break;                //退费
    case 7: deletecard(); break;            //注销卡
    case 8: query_count(); break;            //查询统计
    case 0: 
        printf("=================================\n");
        printf("\n感谢使用，程序即将退出！\n");
        exit(0);
        break;                    //退出系统
    default: {
        printf("\n无效的菜单选项，请重新选择！\n");
        break;
    }
    }
}

