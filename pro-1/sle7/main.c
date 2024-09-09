#include <stdio.h>

int main() {
    // int a, b;

    // printf("请输入两个数：");
    // scanf("%d %d", &a, &b);

    // if (a!=b) {
    //     if (a > b) {
    //         printf("%d > %d\n", a, b);
    //     } else {
    //         printf("%d < %d\n", a, b);
    //     }
    // }

    char isRain, isFree;
    printf("是否有空？[Y/N]");
    scanf("%c", &isFree);

    getchar(); // 处理回车键

    printf("是否下雨?[Y/N]");
    scanf("%c", &isRain);

    if (isFree == 'Y')
    {    if (isRain == 'Y')
            printf("记得带伞哦^_^\n");
    }
    else
    {
        printf("没空！T_T");
    }

    return 0;
}