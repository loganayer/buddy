#ifndef BUDDY_H
#define BUDDY_H

void buddy_init();
void *buddy_alloc(int size);
void buddy_free(void *addr);
void buddy_dump();
void* split(int order, int reqOrder);

#endif // BUDDY_H
