#include <stdio.h>
#include <math.h>

int main() {

    // char str[100];
    // printf("请输入一个字符串： ");
    // scanf("%s", str);
    // printf("%s\n", str);

    int i,j,k;
    i = 1+2;
    j = 1 + 2 *3;
    k = i + j + -1 + pow(2, 3);

    printf("%d\n", k);

    printf("%d\n", 1<2);
    printf("%d\n", 'a' > 'b');
    printf("%d\n", 'a' <= 'b' + 1);

    char ch;
    printf("请输入一个字符：");
    scanf("%c", &ch);
    switch (ch)
    {
    case 'A':
        printf("你的成绩在90分以上！\n");
        break;
    case 'B':
        printf("你的成绩在80分到90之间！\n");
        break;
    case 'C':
        printf("你的成绩在70分到80分之间！\n");
        break;
    case 'D':
        printf("你的成绩在60分到70之间！\n");
        break;
    default:
        break;
    }

    return 0;
}