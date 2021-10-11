#include "heap.h"


//this is the function for creating new io_request
static call_stub_t *
my_stub_new(int app_index, app_info_t* app_info)
{
    call_stub_t *new = NULL;

    glusterfs_fop_t new_fop = GF_FOP_WRITE;

    new = calloc(1, sizeof(*new));

    new->frame = NULL;
    new->wind = "a";
    new->fop = new_fop;
    new->stub_mem_pool = NULL;

    //init QoS info
    new->app_index = app_index;
    //这个地方要根据全局的app_info来计算新的标签
    gettimeofday(&new->rtag, NULL);
	gettimeofday(&new->ptag, NULL);
    gettimeofday(&new->ltag, NULL);

    INIT_LIST_HEAD(&new->list);
    INIT_LIST_HEAD(&new->args_cbk.entries);
out:
    return new;
}

int timevalcmp(struct timeval * t1, struct timeval * t2)
{
    if (t1->tv_sec == t2->tv_sec)
        return (t1->tv_usec == t2->tv_usec) ? 0 :
                (t1->tv_usec > t2->tv_usec) ? 1 : -1;
    return (t1->tv_sec > t2->tv_sec) ? 1 : -1;
}

//最小堆的比较函数，tag小的话放前面
bool find_parent_and_compare(heap_t* heap, call_stub_t* element, heap_name_t heap_name, int p){
    switch (heap_name)
    {
    case reverse:
        if(timevalcmp(&(heap->elements[(p-1)/2]->rTag),&(element->rTag))<0){
            return true;
        }else{
            return false;
        }
        break;
    case propotion:
        if(timevalcmp(&(heap->elements[(p-1)/2]->pTag),&(element->pTag))<0){
            return true;
        }else{
            return false;
        }
        break;
    case limit:
        if(timevalcmp(&(heap->elements[(p-1)/2]->lTag),&(element->lTag))<0){
            return true;
        }else{
            return false;
        }
        break;
    default:
        return true;
        break;
    }
} 

//根据heap_name，更新element的在heap_name指定的heap中的位置
void update_position(call_stub_t* element, heap_name_t heap_name, int p){
    switch (heap_name)
    {
    case reverse:
        element->position_r=p;
        break;
    case propotion:
        element->position_p=p;
        break;
    case limit:
        element->position_l=p;
        break;
    default:
        break;
    }
}

//将element插入到heap_name指定的heap中
bool insertToHeap(heap_t* heap, call_stub_t* element, heap_name_t heap_name){
    //判断是否满了
    if (heap->size >= MAX_REQ){
        printf("out of heap capacity!");
        return false;
    }
    //寻找element应该插入的位置
    int p = ++heap->size;//size+1，并且这个也是元素一开始插入的位置
    for (; find_parent_and_compare(heap, element, heap_name,p); p=(p-1)/2) {
        heap->elements[p] = heap->elements[(p-1)/2];
    }
    heap->elements[p] = element;
    update_position(element, heap_name,p);
    return true;
}

//从p的位置删除一个element
int deleteFromHeap(heap_t* heap, int p, heap_name_t heap_name){
    
}