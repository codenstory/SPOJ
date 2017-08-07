/*
 * official solution
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1000
#define HMAX (MAX*4)
#define DMAX (MAX/10)
#define BASE 1000

struct number { int digits;
                int values[DMAX];};
char names[HMAX][11];
int next_sibling[HMAX];
int first_subord[HMAX];
int withn[HMAX];
int woutn[HMAX];
int active[HMAX];
int root;
struct number with[HMAX];
struct number wout[HMAX];

void bug(void) {
  fprintf(stderr,"There is a bug walking around...\n");
  exit(0);
  }

void zero(struct number *result) {
  result->digits=0;
  }

void unit(struct number *result) {
  result->digits=1;
  result->values[0]=1;
  }

void add(struct number *result, struct number *what) {
  int k;
  while (result->digits<what->digits) result->values[result->digits++]=0;
  for (k=0; k<what->digits; k++) {
    if (k&&(result->values[k-1]>=BASE)) {
      result->values[k]+=result->values[k-1]/BASE;
      result->values[k-1]%=BASE;
      }
    result->values[k]+=what->values[k];
    }
  if (!k) return;
  for (; (result->values[k-1]>=BASE)&&(k<result->digits); k++) {
    result->values[k]+=result->values[k-1]/BASE;
    result->values[k-1]%=BASE;
    }
  if (result->values[k-1]>=BASE) {
    if (k!=result->digits) bug();
    result->values[k]=result->values[k-1]/BASE;
    result->values[k-1]%=BASE;
    result->digits++;
    }
  }

void mul(struct number *result, struct number *what) {
  int i,j;
  struct number aux,what2;
  zero(&what2); add(&what2,result); // copy
  zero(result);
  for (i=0; i<what->digits; i++) {
    for (j=0; j<what2.digits; j++) aux.values[i+j]=what->values[i]*what2.values[j];
    for (j=0; j<i; j++) aux.values[j]=0;
    aux.digits=what2.digits+i;
    add(result,&aux);
    }
  }

void print(struct number *what) {
  int i;
  if (!what->digits) { printf("0"); exit(0); }
  printf("%d",what->values[what->digits-1]);
  for (i=what->digits-2; i>=0; i--) printf("%03d",what->values[i]);
  }

int hash(char *name) {
  int i,r;
  for (i=0, r=0; name[i]; i++) {
    r=(13*r+name[i]-'0')%HMAX;
    }
  return r;
  }

int create(char *name) {
  int i=hash(name);
  while (active[i]) { i++; if (i>=HMAX) i=0; }
  active[i]=1;
  strcpy(names[i],name);
  next_sibling[i]=-1;
  first_subord[i]=-1;
  return i;
  }

int find(char *name) {
  int i=hash(name);
  while (strcmp(name,names[i])&&active[i]) {
    i++; if (i>=HMAX) i=0;
    }
  if (!active[i]) return -1;
  return i;
  }

int read_in(void) {
  int i,j,N,k,children;
  char name[11];
  root=create("1");
  if ( 1 != scanf("%d\n",&N) ) return 0;
  for (i=0; i<N; i++) {
    scanf("%s",name);
    k=find(name);
    scanf("%d",&children);
    while (children--) {
      scanf("%s",name);
      j=create(name);
      next_sibling[j]=first_subord[k];
      first_subord[k]=j;
      }
    } // endfor i
	return 1;
  }

void compute(int which) {
  int i,k;
  struct number aux;
  for (k=first_subord[which]; k!=-1; k=next_sibling[k]) compute(k);
  woutn[which]=0;
  unit(&wout[which]);
  for (k=first_subord[which]; k!=-1; k=next_sibling[k]) {
    woutn[which]+=withn[k];
    mul(&wout[which],&with[k]);
    }
  if (first_subord[which]==-1) {
    withn[which]=0;
    unit(&with[which]);
    return;
    }
  for (k=first_subord[which]; k!=-1; k=next_sibling[k])
    if (withn[k]==woutn[k]) break;
  if (k==-1) {
    withn[which]=woutn[which];
    zero(&with[which]);
    for (k=first_subord[which]; k!=-1; k=next_sibling[k]) {
      unit(&aux);
      for (i=first_subord[which]; i!=-1; i=next_sibling[i])
        mul(&aux,i==k?&wout[i]:&with[i]);
      add(&with[which],&aux);	
      } // endfor k
    add(&with[which],&wout[which]);
    }
  else {
    withn[which]=woutn[which]+1;
    zero(&with[which]);
    for (k=first_subord[which]; k!=-1; k=next_sibling[k]) {
      if (withn[k]!=woutn[k]) continue;
      unit(&aux);
      for (i=first_subord[which]; i!=-1; i=next_sibling[i])
        mul(&aux,i==k?&wout[i]:&with[i]);
      add(&with[which],&aux);	
      } // endfor k
    } // endif k==-1
  }

int main(void) {
  for ( ;read_in(); memset(active,0,sizeof active) ) {
  	compute(root);
  	printf("%d\n",withn[root]);
 	print(&with[root]);
  	printf("\n");
  }
  return 0;
  }
