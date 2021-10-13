#include <glusterfs/heap.h>

static int timevalcmp(struct timeval * t1, struct timeval * t2)
{
    if (t1->tv_sec == t2->tv_sec)
        return (t1->tv_usec == t2->tv_usec) ? 0 :
                (t1->tv_usec > t2->tv_usec) ? 1 : -1;
    return (t1->tv_sec > t2->tv_sec) ? 1 : -1;
}

//最小堆的比较函数，tag小的话放前面
//父亲节点小的话返回true
//该函数还可用于任意两个元素的比较,最右边的参数小即返回true
static bool is_smaller(heap_t* heap, heap_name_t heap_name, int cur, int parent){
    switch (heap_name)
    {
    case reverse:
        if(timevalcmp(&(heap->elements[parent]->rTag),&(heap->elements[cur]->rTag))<0){
            return true;
        }else{
            return false;
        }
        break;
    case propotion:
        if(timevalcmp(&(heap->elements[parent]->pTag),&(heap->elements[cur]->pTag))<0){
            return true;
        }else{
            return false;
        }
        break;
    case limit:
        if(timevalcmp(&(heap->elements[parent]->lTag),&(heap->elements[cur]->lTag))<0){
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
static void update_position(call_stub_t* element, heap_name_t heap_name, int p){
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

static int siftUp(heap_t* heap, heap_name_t heap_name, int start)	//从start到0上滑调整成为最小堆,主要用来插入元素，返回最终插入的位置
{
    int cur = start;
    int parent = (cur - 1) / 2;
    while (cur>0)
    {
        if(is_smaller(heap, heap_name,cur,parent))
            break;
        else
        {
            call_stub_t* temp = heap->elements[cur];
            heap->elements[cur] = heap->elements[parent];
            update_position(heap->elements[cur],heap_name,cur);
            heap->elements[parent] = temp;
            update_position(heap->elements[parent],heap_name,parent);
            cur = parent;
            parent = (parent - 1) / 2;
        }
    }
    //heap->elements[cur] = temp;	//回放temp中暂存的元素
    return cur;
}

static int siftDown(heap_t* heap, heap_name_t heap_name, int start, int end)//用来删除元素及重新堆化
{
    int cur = start;
    int min_child = 2 * cur + 1;
    while (min_child<=end)
    {   //  右边还有一个节点 并且 右边的节点更大
        if (min_child<end&&is_smaller(heap,heap_name,min_child,min_child+1))	//找到左右孩子中最小的一个
				min_child++;

        if(is_smaller(heap,heap_name,min_child,cur))
            break;
        else
        {
            call_stub_t* temp = heap->elements[cur];
            heap->elements[cur] = heap->elements[min_child];
            update_position(heap->elements[cur],heap_name,cur);
            heap->elements[min_child] = temp;//在这一步直接进行交换
            update_position(heap->elements[min_child],heap_name,min_child);
            cur = min_child;
            min_child = 2 * min_child + 1;
        }
    }
    //heap->elements[cur] = temp;	//回放temp中暂存的元素
    return cur;
}

void heapify(heap_t* heap, heap_name_t heap_name){
    currentSize = heap->size;
    int currentPos = (currentSize - 2) / 2;	//找最初调整位置:最后分支结点
    while (currentPos>=0)	//自底向上逐步扩大形成堆
    {
        siftDowm(heap, heap_name, currentPos, currentSize - 1);	//局部自上向下下滑调整
        currentPos--;	//再向前换一个分支结点
    }
}

//将element插入到heap_name指定的heap中
bool insertToHeap(heap_t* heap, heap_name_t heap_name, call_stub_t* element){
    //判断是否满了
    if (heap->size >= MAX_REQ){
        printf("out of heap capacity!\n");
        return false;
    }
    //寻找element应该插入的位置
    int p = heap->size;//元素一开始插入的位置
    heap->elements[p]=element;
    siftUp(heap,heap_name,p); //从p开始向上调整
    ++heap->size;
    return true;
}

//从p的位置删除一个element
bool deleteFromHeap(heap_t* heap, heap_name_t heap_name, int p){
    //先把最后一个元素的位置交换到位置p，再把size--
    last_p = heap->size-1;
    if(p>last_p){
        printf("out of boundary!\n");
        return false;
    }
    if(p==last_p){//如果相等的话，就说明是删除最后一个元素，直接size--返回就行
        heap->size--;
        printf("delete last element!\n");
        return true;
    }
    heap->elements[p] = heap->elements[last_p];
    update_position(heap->elements[p],heap_name,p);
    heap->size--;
    heapify(heap,heap_name);
}