#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {

    char ch[100];
    printf("请输入一个字符串：");
    scanf("%s", ch);

    printf("输入的字符串是：%s\n", ch); 

    int length = strlen(ch);
    printf("字符串的长度为： %d\n", length);

    char str2[100] = "Hello";
    if (strcmp(ch, str2) == 0) {
        printf("两字符串完全相同\n");
    } else {
        printf("两个字符串完全不相等\n");
    }

    char str3[100];
    strcpy(str3, ch);
    strcat(str3, " World");
    printf("拼接字符串：%s\n", str3);

    return 0;
}