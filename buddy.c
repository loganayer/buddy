/**
 * Buddy Allocator
 *
 * For the list library usage, see http://www.mcs.anl.gov/~kazutomo/list/
 */

/**************************************************************************
 * Conditional Compilation Options
 **************************************************************************/
#define USE_DEBUG 0

/**************************************************************************
 * Included Files
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buddy.h"
#include "list.h"

/**************************************************************************
 * Public Definitions
 **************************************************************************/
#define MIN_ORDER 12
#define MAX_ORDER 20

#define PAGE_SIZE (1<<MIN_ORDER)
/* page index to address */
#define PAGE_TO_ADDR(page_idx) (void *)((page_idx*PAGE_SIZE) + g_memory)

/* address to page index */
#define ADDR_TO_PAGE(addr) ((unsigned long)((void *)addr - (void *)g_memory) / PAGE_SIZE)

/* find buddy address */
#define BUDDY_ADDR(addr, o) (void *)((((unsigned long)addr - (unsigned long)g_memory) ^ (1<<o)) \
		+ (unsigned long)g_memory)

#if USE_DEBUG == 1
#  define PDEBUG(fmt, ...) \
	fprintf(stderr, "%s(), %s:%d: " fmt,			\
			__func__, __FILE__, __LINE__, ##__VA_ARGS__)
#  define IFDEBUG(x) x
#else
#  define PDEBUG(fmt, ...)
#  define IFDEBUG(x)
#endif

/**************************************************************************
 * Public Types
 **************************************************************************/
typedef struct {
	struct list_head list;
	/* TODO: DECLARE NECESSARY MEMBER VARIABLES */
	int order; //alloc step 1
	int pageIndex; //split step 2
	char* buddy_address; //split step 4
	int isFree;
} page_t;

/**************************************************************************
 * Global Variables
 **************************************************************************/
/* free lists*/
struct list_head free_area[MAX_ORDER+1];

/* memory area */
char g_memory[1<<MAX_ORDER];

/* page structures */
page_t g_pages[(1<<MAX_ORDER)/PAGE_SIZE];

/**************************************************************************
 * Public Function Prototypes
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**
 * Initialize the buddy system
 */
void buddy_init()
{
	int i;
	int n_pages = (1<<MAX_ORDER) / PAGE_SIZE;
	for (i = 0; i < n_pages; i++) {
		INIT_LIST_HEAD(&g_pages[i].list);
		/* TODO: INITIALIZE PAGE STRUCTURES */
		g_pages[i].pageIndex = i;
		g_pages[i].buddy_address = PAGE_TO_ADDR(i);
		g_pages[i].isFree = 0; //0 means free, 1 means not free
	}

	/* initialize freelist */
	for (i = MIN_ORDER; i <= MAX_ORDER; i++) {
		INIT_LIST_HEAD(&free_area[i]);
	}

	/* add the entire memory as a freeblock */
	list_add(&g_pages[0].list, &free_area[MAX_ORDER]);
}

/**
 * Allocate a memory block.
 *
 * On a memory request, the allocator returns the head of a free-list of the
 * matching size (i.e., smallest block that satisfies the request). If the
 * free-list of the matching block size is empty, then a larger block size will
 {
 * further splitted while the right block will be added to the appropriate
 * free-list.
 *
 * @param size size in bytes
 * @return memory block address
 */

void *buddy_alloc(int size)
{
	int order;
	order = ceil(log2(size));

	/* TODO: IMPLEMENT THIS FUNCTION
	   1. Ascertain the free-block order which can satisfy the requested size. The block order for size x is ceil ( log2 (x))
Example: 60k -> block-order = ceil ( log2 (60k)) = ceil ( log2 (k x 2^5 x 2^10)) = order-16
2. Iterate over the free-lists; starting from the order calculated in the above step. If the free-list at the
required order is not-empty, just remove the first page from that list and return it to caller to satisfy the request
3. If the free-list at the required order is empty, find the first non-empty free-list with order > required-order. Lets say that such a list exists at order-k
4. Remove a page from the order-k list and repeatedly break the page and populate the respective free-lists until
the page of required-order is obtained. Return that page to caller (It would be good to encase this functionality in a separate function e.g. split)
5. If a non-empty free-list is not found, this is an error
	 */
	return NULL;
}

/**
 * Free an allocated memory block.
 *
 * Whenever a block is freed, the allocator checks its buddy. If the buddy is
 * free as well, then the two buddies are combined to form a bigger block. This
 * process continues until one of the buddies is not free.
 *
 * @param addr memory block address to be freed
 */
void buddy_free(void *addr)
{
	
	int Buddyaddress = BUDDY_ADDR(addr,19);
	for(int i = 0; i <= 20; i++)
	{
		if(i == MAX_ORDER)
		{
			list_add(&g_pages[0].list, &free_area[MAX_ORDER]);
		}
	//	else if(isFree == '0'){
			//merge(block address, buddy address)
		//}
	}
	//if(isFree)
	/* TODO: IMPLEMENT THIS FUNCTION
	   1. Calculate the address of the buddy
	   2. If the buddy is free, merge the two blocks i.e. remove the buddy from its free-list, update the order of the page-at-hand and add the page to the relevant free-list
	   3. Do step-2 repeatedly until no merging is possible
	   a. The buddy is not free
	   b. The max order is reached
	 */
}

/**
 * Print the buddy system status---order oriented
 *
 * print free pages in each order.
 */
void buddy_dump()
{
	int o;
	for (o = MIN_ORDER; o <= MAX_ORDER; o++) {
		struct list_head *pos;
		int cnt = 0;
		list_for_each(pos, &free_area[o]) {
			cnt++;
		}
		printf("%d:%dK ", cnt, (1<<o)/1024);
	}
	printf("\n");
}
void split (int order)
{

	//1
	//page_t *page_Structure = &g_pages[ADDR_TO_PAGE(BUDDY_ADDR(PAGE_TO_ADDR(index), (order - 1)))];
	//2
	//3
	//4
	//5
	//6
}
/*split (order):
  1. Get the page structure from the free-list node (Hint: Recall the method to get the payload pointer from the list pointer which can come in handy here. Remember that the free-list[order] node is embedded inside the page-structure as list. You may want to use the page_from_head macro here)
  2. Get the index of the page structure obtained above (Hint: index_from_page)
  3. Now you want to split this page-block of given order into two equally sized blocks of order - 1. Note that the address of the first of these child blocks will be the same as the parent block. You only need to calculate the address of the buddy
  4. Calculate the address of the buddy from the address of the page you have. Hint: Lets say that the page you have; has index i.
  a. Calculate the absolute address of the page using its index and order (page_to_addr might be useful here
  b. Calculate the address of the buddy using the address calculated above (buddy_addr is useful here)
  c. Convert the address of buddy into page-index (addr_to_page)
  5. Populate the fields of both pages appropriately (depending upon what you are keeping in your page-structure. I would suggest that you keep at-least the order of the page in the page-structure)
  6. Add both pages to the free-lists at order - 1 (Note: The buddy page can simply be added to the list. The original page will have to be removed from its current list and moved to the list at order - 1. list_move macro may come in handy for this)
 */

/*merge
  method is quite similar to split. If you can correctly implement split, you should not have any problem implementing merge.
 */
