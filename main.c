#include <time.h>
#include "klib-master/kseq.h"  
#include <zlib.h>  
#include <stdio.h>
#include <string.h>   
#define M 14557
KSEQ_INIT(gzFile, gzread)  
  
typedef struct seq
    {
    int value;
    int num;
    char index;
    struct seq *next1;
    struct seq *next2;
    }*Seq,Sequence;

int main(int argc, char *argv[]) 
{
    gzFile fp;  
    kseq_t *seq;
    long seqs = 0;
    long bases = 0;  
    int l;  
    if (argc == 1) {  
        fprintf(stderr, "Usage: %s <in.seq>\n", argv[0]);  
        return 1;  
    }  
    fp = gzopen(argv[1], "r"); // STEP 2: open the file handler  
    seq = kseq_init(fp); // STEP 3: initialize seq  

    int k,j,i;
    int num,n;
    int m=0;
    int pos=1;
    char s[200];
    Seq seqarray[M]={NULL};
    printf("请输入k值：\n");
    scanf("%d",&k);

    FILE *fp;
    if((fp=fopen("h.fa","r"))==NULL)
    {
    printf("File open error!\n");
    exit(1);
    }


while(!feof(fp))
{
    fscanf(fp,"%*s%s%d%d",s,&num,&j);
    fgetc(fp);
    j=0;
    m=0;
    pos=1;

    for(i=0;i<k;i++) //A:00 T:01 C:10 G:11
    {
//建立索引的时间与k值有关
    switch(fgetc(fp))
    {
    case 'A':
        j+=2;
        break;
    case 'T':
        m+=(int)pow(2,j);
        j+=2;
        break;
    case 'C':
        j++;
        m+=(int)pow(2,j);
        j++;
        break;
    case 'G':
        m+=(int)pow(2,j);
        j++;
        m+=(int)pow(2,j);
        j++;
        break;
    default:
        break;
    }
    }
    n=m%M;

    initNode(&seqarray[n],n);
    addNode(&seqarray[n],m,num,pos);
    pos++;
    while(pos<=100-k)
    {
        j-=2;
        m>>=2;
        switch(fgetc(fp))
        {
        case 'A':
            j+=2;
            break;
        case 'T':
            m+=(int)pow(2,j);
            j+=2;
            break;
        case 'C':
            j++;
            m+=(int)pow(2,j);
            j++;
            break;
        case 'G':
            m+=(int)pow(2,j);
            j++;
            m+=(int)pow(2,j);
            j++;
            break;
        default:
            break;
        }
        n=m%M;
        initNode(&seqarray[n],n);
        addNode(&seqarray[n],m,num,pos);
        pos++;
    }
    fgets(s,50,fp);

}

    getchar();
    printf("请输入要查询的K串\n");
    gets(s);
    m=0;
    j=0;
    for(i=0;i<k;i++)
    switch(s[i])
    {
        case 'A':
        j+=2;
        break;
        case 'T':
        m+=(int)pow(2,j);
        j+=2;
        break;
        case 'C':
        j++;
        m+=(int)pow(2,j);
        j++;
        break;
        case 'G':
        m+=(int)pow(2,j);
        j++;
        m+=(int)pow(2,j);
        j++;
        break;
        default:
        break;
    }

    n=m%M;
    int count=0;
    consultNode(seqarray[n],m,&count);
    printf("*****查询到记录%d条*****\n",count);
    fclose(fp);

    for(j=0;j<M;j++)//释放内存从前往后
    {         
        if(seqarray[j])
        {
        Seq p=seqarray[j]->next1;
        Seq q;
        Seq q1,q2;
        while(p)
        {
        q=p->next1;
        q1=p->next2;
        while(q1)
        {
        q2=q1->next2;
        free(q1);
        q1=q2;
        }
        free(p);
        p=q;
        }
        free(seqarray[j]);
        }
    }
    return 0;
}
