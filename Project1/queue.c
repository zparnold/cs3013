#include<stdio.h>
#include<stdlib.h>
#define QUENE_SIZE 10

typedef struct {int num; pid_t pid;} job;

struct queue_structure{
  unsigned int queue_front, queue_rear;
  job queue_array[10];
};



void enqueue(struct queue_structure *,job newJob);
int dequeue(struct queue_structure *);
int queue_full(struct queue_structure *);
int queue_empty(struct queue_structure *);

void enqueue(struct queue_structure *qs, job newJob){   
  if(qs -> queue_front == qs -> queue_rear){
    qs->queue_front = qs->queue_rear = -1;
  }
  else{
    (qs -> queue_rear)++;
    qs->queue_array[qs->queue_rear] = newJob;    
  }  
}
int dequeue(struct queue_structure *qs){
  int i;
  if(qs->queue_front == qs->queue_rear){
    qs->queue_front = qs->queue_rear = -1; 
  }
  else{
    for(i = qs->queue_front; i < qs->queue_rear ; i++){
      qs->queue_array[i] = qs->queue_array[i + 1];
    }
  }
}
int queue_full(struct queue_structure *qs){
  if((qs->queue_rear == 10) && (qs->queue_front == 0)){
    return 1;
  }
  else{
    return 0;
  }
int queue_empty(struct queue_structure *qs){
  if((qs->queue_rear && qs->queue_front) == -1){
    return 1;
  }
  else{
    return 0;
  }
}

}
