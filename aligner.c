#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSEQ 1000
#define GAP_CHAR '-'

// 对空位的罚分是线性的
struct Unit {
    int W1;   // 是否往上回溯一格
    int W2;   // 是否往左上回溯一格
    int W3;   // 是否往左回溯一格
    float M;      // 得分矩阵第(i, j)这个单元的分值，即序列s(1,...,i)与序列r(1,...,j)比对的最高得分
};
typedef struct Unit *pUnit;

void strUpper(char *s);
float max3(float a, float b, float c);
float getFScore(char a, char b);
void printAlign(pUnit** a, const int i, const int j, char* s, char* r, char* saln, char* raln, int n);
void align(char *s, char *r);

int main() {
    char s[MAXSEQ];
    char r[MAXSEQ];
    printf("The 1st seq: ");
    scanf("%s", s);
    printf("The 2nd seq: ");
    scanf("%s", r);
    align(s, r);
    return 0;
}

void strUpper(char *s) {
    while (*s != '\0') {
        if (*s >= 'a' && *s <= 'z') {
            *s -= 32;
        }
        s++;
    }
}

float max3(float a, float b, float c) {
    float f = a > b ? a : b;
    return f > c ? f : c;
}

// 替换矩阵：match分值为5，mismatch分值为-4
// 数组下标是两个字符的ascii码减去65之后的和
float FMatrix[] = {
    5, 0, -4, 0, 5, 0, -4, 0, -4, 0,
    0, 0, 5, 0, 0, 0, 0, 0, 0, -4,
    0, -4, 0, 0, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 5
};

float getFScore(char a, char b) {
    return FMatrix[a + b - 'A' - 'A'];
}

void printAlign(pUnit** a, const int i, const int j, char* s, char* r, char* saln, char* raln, int n) {
    int k;
    pUnit p = a[i][j];
    if (! (i || j)) {   // 到矩阵单元(0, 0)才算结束，这代表初始的两个字符串的每个字符都被比对到了
        for (k = n - 1; k >= 0; k--)
            printf("%c", saln[k]);
        printf("\n");
        for (k = n - 1; k >= 0; k--)
            printf("%c", raln[k]);
        printf("\n\n");
        return;
    }
    if (p->W1) {    // 向上回溯一格
        saln[n] = s[i - 1];
        raln[n] = GAP_CHAR;
        printAlign(a, i - 1, j, s, r, saln, raln, n + 1);
    }
    if (p->W2) {    // 向左上回溯一格
        saln[n] = s[i - 1];
        raln[n] = r[j - 1];
        printAlign(a, i - 1, j - 1, s, r, saln, raln, n + 1);
    }
    if (p->W3) {    // 向左回溯一格
        saln[n] = GAP_CHAR;
        raln[n] = r[j - 1];
        printAlign(a, i, j - 1, s, r, saln, raln, n + 1);
    }
}

void align(char *s, char *r) {
    int i, j;
    int m = strlen(s);
    int n = strlen(r);
    float gap = -2.5;     // 对空位的罚分
    float m1, m2, m3, maxm;
    pUnit **aUnit;
    char* salign;
    char* ralign;
    // 初始化
    if ((aUnit = (pUnit **) malloc(sizeof(pUnit*) * (m + 1))) == NULL) {
        fputs("Error: Out of space!\n", stderr);
        exit(1);
    }
    for (i = 0; i <= m; i++) {
        if ((aUnit[i] = (pUnit *) malloc(sizeof(pUnit) * (n + 1))) == NULL) {
            fputs("Error: Out of space!\n", stderr);
            exit(1);     
        }
        for (j = 0; j <= n; j++) {
            if ((aUnit[i][j] = (pUnit) malloc(sizeof(struct Unit))) == NULL) {
                fputs("Error: Out of space!\n", stderr);
                exit(1);     
            }
            aUnit[i][j]->W1 = 0;
            aUnit[i][j]->W2 = 0;
            aUnit[i][j]->W3 = 0;
        }
    }
    aUnit[0][0]->M = 0;
    for (i = 1; i <= m; i++) {
        aUnit[i][0]->M = gap * i;
        aUnit[i][0]->W1 = 1;
    }
    for (j = 1; j <= n; j++) {
         aUnit[0][j]->M = gap * j;
        aUnit[0][j]->W3 = 1;
    }
    // 将字符串都变成大写
    strUpper(s);
    strUpper(r);
    // 动态规划算法计算得分矩阵每个单元的分值
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            m1 = aUnit[i - 1][j]->M + gap;
            m2 = aUnit[i - 1][j - 1]->M + getFScore(s[i - 1], r[j - 1]);
            m3 = aUnit[i][j - 1]->M + gap;
            maxm = max3(m1, m2, m3);
            aUnit[i][j]->M = maxm;
            if (m1 == maxm) aUnit[i][j]->W1 = 1;
            if (m2 == maxm) aUnit[i][j]->W2 = 1;
            if (m3 == maxm) aUnit[i][j]->W3 = 1;
        }
    }
/*
    // 打印得分矩阵
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++)
            printf("%f ", aUnit[i][j]->M);
        printf("\n");
    }
*/
    printf("max score: %f\n", aUnit[m][n]->M);
    // 打印最优比对结果，如果有多个，全部打印
    // 递归法
    if ((salign = (char*) malloc(sizeof(char) * (m + n + 1))) == NULL) {
        fputs("Error: Out of space!\n", stderr);
        exit(1);
    }
    if ((ralign = (char*) malloc(sizeof(char) * (m + n + 1))) == NULL) {
        fputs("Error: Out of space!\n", stderr);
        exit(1);
    }
    printAlign(aUnit, m, n, s, r, salign, ralign, 0);
    // 释放内存
    free(salign);
    free(ralign);
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            free(aUnit[i][j]);
        }
        free(aUnit[i]);
    }
    free(aUnit);
}