#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define OK 1
#define ERROR 0
#define MVNum 14111
#define LEN(x) sizeof(x)/sizeof(x[0])
#define ALL 12510

typedef int Status;
typedef int VerTexType;
//save net
typedef struct ArcNode{
    char word[10];
    int adjvex;
    struct ArcNode *nextarc;
}ArcNode;
typedef struct VNode{
    VerTexType data;
    ArcNode *firstarc;
}VNode,AdjList[MVNum];
typedef struct{
    AdjList vextices;
}ALGraph;

//save point
typedef struct{
    int id;
    int area;
    char name[10];
    int left_or_right;
    int Lock;
    int gain;
    int used;
}Point;
Point point[ALL];

int count;
int TotalGain = 0;
int maxAreaV = 0;
int randomNUm = 0;
/*........................*/
int key = 0;
void createPointStructure(Point *point, char input[12]){
    int num;
    char *result = NULL;
    result = (char*)malloc(sizeof(char*));
    result = strtok(input, " " );
    //可以用随机数给分配
    int randomLR = rand()%2;
    point->left_or_right = randomLR;
    //继续调用strtok，分解剩下的字符串
    strcpy(point->name,result);
    result=strtok(NULL," ");
    //转int
    num = atoi (result);
    //存一下最大点
    if(num>maxAreaV){
        maxAreaV = num;
    }
    point->area = num;
    point->Lock = 0;
}

//创建邻接表结构
int idx = -1;
Status CreateUDG(ALGraph *G, char input[18])
{
    ArcNode *p1, *p;
    char *result = NULL;
    result = (char*)malloc(sizeof(char*));
    char *ret, *ret2;
    ret = strrchr(input, 's');
    ret2 = strrchr(input, 'p');
    //切割
    result = strtok( input, " " );
    //create
    if(ret!=NULL){
        if(ret2!=NULL){
            return OK;
        }else{
            idx++;
            G->vextices[idx].data = idx;
            G->vextices[idx].firstarc = NULL;
            p = (ArcNode *)malloc(sizeof(ArcNode));
            strcpy(p->word, result);
            
            p->nextarc = G->vextices[idx].firstarc;
            G->vextices[idx].firstarc = p;
        }
        
    }else{
        if(ret2!=NULL){
            idx--;
            return OK;
        }else{
            p1 = (ArcNode *)malloc(sizeof(ArcNode));
           strcpy(p1->word, result);
           p1->nextarc = G->vextices[idx].firstarc;
           G->vextices[idx].firstarc = p1;
        }
    }
    count = count + 1;
    return OK;
}

int oriareaL = 0;
int oriareaR = 0;
int areaV = 0;
int minAreaA,maxAreaA,area_result;
void calOriAreaAandB(){
    float ratio = 0.01;
    for(int u = 0;u<ALL;u++){
        areaV = areaV + point[u].area;
        if(point[u].left_or_right==1){
            oriareaL = oriareaL + point[u].area;
        }else{
            oriareaR = oriareaR + point[u].area;
        }
    }
    ratio = (float)oriareaL / (oriareaL + oriareaR);
    minAreaA = (ratio * areaV) - maxAreaV;
    maxAreaA = (ratio * areaV) + maxAreaV;
}


int judgeAreaAAfterMove(int p){
    int LR = point[p].left_or_right;
    int areaPoint = point[p].area;
    if(LR == 1){
        oriareaL = oriareaL - areaPoint;
    }else{
        oriareaL = oriareaL + areaPoint;

    }
    if(oriareaL<minAreaA || oriareaL>maxAreaA){
        //不满足面积限制
        area_result = 0;
    }else{
        //满足面积限制
        area_result = 1;
    }

    return area_result;
}

void netFileRead(ALGraph *G){
    FILE *fp;
    char input[18];
    fp = fopen("/Users/jiang/Desktop/ibm01.txt","r");
    if(fp == NULL)
    {
        printf("Open filefailure!");
        exit(1);
    }
    else
    {
        while(fgets(input, 18, fp) != NULL){
            if(strrchr(input, 'p') != NULL){
                continue;
            }else{
                CreateUDG(G, input);
            }
        }
        fclose(fp);
    }
}

void areFileRead(){
    FILE *fp;
    char input[18];
    fp = fopen("/Users/jiang/Desktop/ibm01.are","r");
    if(fp == NULL)
    {
        printf("Open filefailure!");
        exit(1);
    }
    else
    {
        while(fgets(input, 18, fp) != NULL){
            if(strrchr(input, 'p')!=NULL){
                continue;
            }else{
                point[key].id = key;
                createPointStructure(&point[key], input);
                key++;
            }
        }
        fclose(fp);
    }
}

int calculateGain(ALGraph *G, char* content)
{
   ArcNode *p;
   int q;
   int FS = 0,TS = 0;
   bool isCellInsideGraph = false;
    //
   for (q=0;q<14015;q++){
       p = G->vextices[q].firstarc;
       int mid = 0;
       int count2 = 0;
       int count = 0;
       bool isCellInsideNet = false;
       while(p!=NULL){
           count2++;
           int middle = 0;
           //
           for(int idex = 0; idex<idx; idex++)
           {
               if(strcmp(point[idex].name,content) == 0){
                   mid = point[idex].left_or_right;
               }
               if(strcmp(point[idex].name,p->word) == 0){
                   middle = point[idex].left_or_right;
                   break;
               }
           }
           count = count + middle;
           if(strcmp(p->word, content)==0){
               isCellInsideNet = true;
               isCellInsideGraph = true;
               //mid = L_or_R(content) ;
           }
           p=p->nextarc;
       }
       if(mid==1 && count == 1 && isCellInsideNet==true){
           FS = FS + 1;
       }else if(mid==0 && count == count2 - 1 && isCellInsideNet==true){
           FS = FS + 1;
       }
       if(count == count2 &&  isCellInsideNet==true){
           TS = TS + 1;
       }else if(count == 0 && isCellInsideNet==true){
           TS = TS + 1;
       }
   }
    if(isCellInsideGraph == true){
        return FS - TS;
    }
    return -999;
}

int thepointNumYouMove = 0;
int calMaxGain(ALGraph *G){
    int maxGain = -10;
    char* nextChange;
    int i;
    for(i = 0; i < 12752;i++){
        if(point[i].Lock == 1){
            continue;
        }
        point[i].gain = calculateGain(G, point[i].name);
        point[i].used = 0;
        printf("Cell %s ‘s gain is: %d \n",point[i].name, point[i].gain);
        if(point[i].gain > maxGain){
            nextChange = point[i].name;
            maxGain = point[i].gain;
            thepointNumYouMove = i;
        }
    }
    return maxGain;
}

int findAnotherPoint(int num){
    int another = -1;
    while(num>-1){
        for(int ii = 0; ii<12752; ii++){
            if(point[ii].used == 1){
                continue;
            }else{
                if(point[ii].gain == num){
                    another = ii;
                    return another;
                }
            }
        }
        num--;
    }
    return another;
}
void movePoint(int thepointNumYouMove){
    printf("now we want to move %s \n", point[thepointNumYouMove].name);
    //swap point
    if(point[thepointNumYouMove].left_or_right == 1){
        point[thepointNumYouMove].left_or_right = 0;
    }else{
        point[thepointNumYouMove].left_or_right = 1;
    }
    point[thepointNumYouMove].Lock = 1;
    TotalGain = TotalGain + point[thepointNumYouMove].gain;
    printf("Lock Cell :%s \n",point[thepointNumYouMove].name);
    printf("Now Total Gain is %d !! \n",TotalGain);
}

int main(void)
{
    srand(time(0));
    areFileRead();
    printf("we have input .are document ！ \n");
    ALGraph G;
    netFileRead(&G);
    printf("we have input .net document ！ \n");
    printf("The adjacency list structure is created \n");
    calOriAreaAandB();
    int theNumMaxgain = 1;
    int totalNum = 0;
    bool end = false;
    while(totalNum<12752){
        theNumMaxgain = calMaxGain(&G);
        printf("The maxGain is: %d and it is %s \n", theNumMaxgain, point[thepointNumYouMove].name);
        //check area constrain
        while(judgeAreaAAfterMove(thepointNumYouMove) != 1){
            printf("%s unsatisfy area constrain\n",  point[thepointNumYouMove].name);
            point[thepointNumYouMove].used = 1;
            int nowpoint = findAnotherPoint(point[thepointNumYouMove].gain);
            if(nowpoint != -1){
                thepointNumYouMove = nowpoint;
            }else{
                printf("can't find another point satisfy area constrain!\n");
                end = true;
                break;
            }
        }
        if(end!=true){
            movePoint(thepointNumYouMove);
        }
        totalNum++;
   
    }
    return 0;
}


