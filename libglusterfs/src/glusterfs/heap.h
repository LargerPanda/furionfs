#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <glusterfs/call-stub.h>

#define MAX_APP 100
#define MAX_REQ 10000
 
typedef enum heap_name{reserve=0, propotion, limit} heap_name_t;

typedef struct app_info{
    //app的三个指标值
    double app_r[MAX_APP];
    double app_p[MAX_APP];
    double app_l[MAX_APP];

    //app的上个请求的标签
    struct timeval last_r_of_app[MAX_APP];
    struct timeval last_p_of_app[MAX_APP];
    struct timeval last_l_of_app[MAX_APP];

}app_info_t;

typedef struct req_heap
{
    call_stub_t* elements[MAX_REQ];//从0开始放置
    int size;
    heap_name_t type;
}heap_t;

// void haha(){
//     printf("hahahaa");
// }

// void haha1();


int timevalcmp(struct timeval * t1, struct timeval * t2);

//向外提供堆化、插入、和删除操作
//取元素就是简化版的删除操作
void heapify(heap_t* heap, heap_name_t heap_name);

bool insertToHeap(heap_t* heap, heap_name_t heap_name, call_stub_t* element);

call_stub_t* deleteFromHeap(heap_t* heap, heap_name_t heap_name, int p);

#endif