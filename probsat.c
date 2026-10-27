// probSAT (Balint&Schoening) for 3-SAT. usage: probsat file.cnf seed maxsec
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
static unsigned long long rs;
static inline unsigned rnd(){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return (unsigned)(rs>>11);}
static inline double rndd(){return (rnd()&0xffffff)/16777216.0;}
int K=3,n,m,*cl,*nt,*fpos,*fl,nf,*tc,**occ,*no,*brk;char*a;
int main(int c,char**v){FILE*f=fopen(v[1],"r");rs=atoll(v[2])*2654435761ULL+1;double maxs=atof(v[3]);double cb=c>4?atof(v[4]):2.06,eps=0.9;
 char line[256];while(fgets(line,256,f)){if(line[0]=='p'){sscanf(line,"p cnf %d %d",&n,&m);break;}}{long pos=ftell(f);int x,k=0;while(fscanf(f,"%d",&x)==1&&x)k++;K=k;fseek(f,pos,SEEK_SET);}
 cl=malloc(sizeof(int)*K*m);for(int i=0;i<m;i++){int x,j=0;while(j<K&&fscanf(f,"%d",&x)==1){if(x)cl[K*i+j++]=x;}fscanf(f,"%d",&x);}
 no=calloc(2*n+2,sizeof(int));for(int i=0;i<K*m;i++)no[cl[i]+n]++;occ=malloc(sizeof(int*)*(2*n+1));int*cnt=calloc(2*n+1,sizeof(int));
 for(int i=0;i<=2*n;i++)occ[i]=malloc(sizeof(int)*(no[i]+1));for(int i=0;i<m;i++)for(int j=0;j<K;j++){int l=cl[K*i+j]+n;occ[l][cnt[l]++]=i;}
 double lut[64];for(int i=0;i<64;i++)lut[i]=(K>3)?pow(cb,-i):pow(eps+i,-cb);
 a=malloc(n+1);nt=malloc(sizeof(int)*m);tc=malloc(sizeof(int)*m);fl=malloc(sizeof(int)*m);fpos=malloc(sizeof(int)*m);brk=calloc(n+1,sizeof(int));
 clock_t t0=clock();long long flips=0;
 for(int i=1;i<=n;i++)a[i]=rnd()&1;if(c>5){FILE*g=fopen(v[5],"r");for(int i=1;i<=n;i++){int ch=fgetc(g);a[i]=ch=='1';}fclose(g);}nf=0;
 for(int i=0;i<m;i++){nt[i]=0;for(int j=0;j<K;j++){int l=cl[K*i+j];if((l>0)==a[abs(l)]){nt[i]++;tc[i]=abs(l);}}
  if(nt[i]==0){fpos[i]=nf;fl[nf++]=i;}else if(nt[i]==1)brk[tc[i]]++;}
 double pr[16];
 while(nf>0){
  if((flips&0xfffff)==0&&(double)(clock()-t0)/CLOCKS_PER_SEC>maxs){printf("s UNKNOWN %d\n",nf);return 1;}
  int ci=fl[rnd()%nf];double s=0;for(int j=0;j<K;j++){int b=brk[abs(cl[K*ci+j])];pr[j]=lut[b<64?b:63];s+=pr[j];}
  double r=rndd()*s;int j=0;for(;j<K-1;j++){r-=pr[j];if(r<=0)break;}
  int x=abs(cl[K*ci+j]);a[x]^=1;flips++;
  int tl=a[x]?x+n:-x+n,fls=a[x]?-x+n:x+n;
  for(int k=0;k<no[tl];k++){int i=occ[tl][k];nt[i]++;if(nt[i]==1){int lst=fl[--nf];fl[fpos[i]]=lst;fpos[lst]=fpos[i];tc[i]=x;brk[x]++;}else if(nt[i]==2)brk[tc[i]]--;}
  for(int k=0;k<no[fls];k++){int i=occ[fls][k];nt[i]--;if(nt[i]==0){fpos[i]=nf;fl[nf++]=i;brk[x]--;}else if(nt[i]==1){for(int q=0;q<K;q++){int l=cl[K*i+q];if((l>0)==a[abs(l)]){tc[i]=abs(l);break;}}brk[tc[i]]++;}}
 }
 printf("s SATISFIABLE flips %lld time %.1f\nv ",flips,(double)(clock()-t0)/CLOCKS_PER_SEC);for(int i=1;i<=n;i++)putchar('0'+a[i]);putchar('\n');return 0;}
