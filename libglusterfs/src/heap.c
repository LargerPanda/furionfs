#include "heap.h"

static call_stub_t *
my_stub_new()
{
    call_stub_t *new = NULL;

    glusterfs_fop_t new_fop = GF_FOP_WRITE;

    new = calloc(1, sizeof(*new));

    new->frame = NULL;
    new->wind = "a";
    new->fop = new_fop;
    new->stub_mem_pool = NULL;
    INIT_LIST_HEAD(&new->list);
    INIT_LIST_HEAD(&new->args_cbk.entries);
out:
    return new;
}