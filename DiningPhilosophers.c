// ยศวิชญ์ จันสิงห์ 65025772
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int processid[5];
bool finish[5];
int resource[5];
int length = sizeof(processid) / sizeof(processid[0]);
void eat(int id){
    printf("process %d eat\n",id);
    finish[id] = true;
    resource[(id+1)%length]=-1;
    resource[id] = -1;
}


void eatSpaghetti(int id){
  if((resource[(id+1)%length]==(-1)&&resource[id]==(-1)) || (resource[(id+1)%length]==id||resource[id]==id))
  {     if(id%2==0){
            if(resource[(id+1)%length]==(-1) && !finish[id]){
                printf("Process %d pick up Rigth Fork\n",id);
                resource[(id+1)%length]=id;


            }else if(resource[id]==(-1)&&!finish[id]){
                            resource[id]=id;
                            printf("Process %d pick up Left Fork\n",id);

                }
        }
        else{
            if(resource[id]==(-1)&&!finish[id]){
                          resource[id]=id;
                         printf("Process %d pick up Left Fork\n",id);

            }else if(resource[(id+1)%length]==(-1) && !finish[id]){
                 printf("Process %d pick up Rigth Fork\n",id);
                 resource[(id+1)%length]=id;


                }
            
        }
    }else{
        printf("Process %d wait next round\n",id);
    }
    if((resource[(id+1)%length]==id)&&(resource[id]==id))
             {
               eat(id);
             }                  
}

int main() {
    for(int i =0;i<length;i++)
    {
        processid[i]=i ;
        finish[i]=false;
        resource[i]=-1;
    }
   while (true) {
        bool all_finished = true;
        for (int i = 0; i < length; i++) {
            eatSpaghetti(i);
            if (!finish[i]) {
                all_finished = false;
            }
        }

        if (all_finished) {
            break; 
        }
    }
    printf("everybody eat sphagetti finish\n");
    

    return 0;
}