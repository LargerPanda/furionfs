#ifndef HEAP_H
#define HEAP_H


#include "call-stub.h"

#define MAX_APP 100
#define MAX_REQ 10000
 
enum heap_name_t{reserve=0, propotion, limit};

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
}heap_t;

#endif