#include "klib-master/ksort.h"
#include "klib-master/kseq.h"
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

void initNode(Seq *r,int i)
{
    if(*r==NULL)
    {
    *r=(Seq)malloc(sizeof(Sequence));
    (*r)->next1=NULL;
    (*r)->next2=NULL;
    }
}

void addNode(Seq *r,int v,int n,int i)
{
    Seq s=(*r)->next1;
    Seq p=*r;
    if(s)
    {
        while(s&&v>s->value)
        {
        p=s;
        s=s->next1;
        }
        if(s&&v==s->value)
        {
        Seq qsub=(Seq)malloc(sizeof(Sequence));
        qsub->value=v;
        qsub->index=(char)i;
        qsub->num=n;
        qsub->next1=NULL;
        qsub->next2=s->next2;
        s->next2=qsub;
        }
        else
        {
        Seq q=(Seq)malloc(sizeof(Sequence));
        q->next1=s;
        q->value=v;
        q->index=(char)i;
        q->num=n;
        q->next2=NULL;
        p->next1=q;
        }
    }
    else
    {
        Seq q=(Seq)malloc(sizeof(Sequence));
        q->next1=s;
        q->value=v;
        q->index=(char)i;
        q->num=n;
        q->next2=NULL;
        (*r)->next1=q;
    }
}

void consultNode(Seq s,int v,int *count)
{
    Seq r;
    if(s)
    r=s->next1;
    while(r&&v>=r->value)
    {
    if(v==r->value)
        {
        (*count)++;
        printf("%d,%d\n",r->num,r->index);
        Seq q=r->next2;
        while(q)
        {
        (*count)++;
        printf("%d,%d\n",q->num,q->index);
        q=q->next2;
        }
        }
    r=r->next1;
    }
}
