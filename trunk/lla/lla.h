#ifndef _LLA_H_
#define _LLA_H_

#define MAX_NODE 8
#define MAX_ARRAY 8

#define EMPTY -1

struct lla_node{
	int next;
	int data_index;
};
	
struct lla_head{
	struct lla_node node[MAX_NODE];
	int array[MAX_ARRAY];
	int tail;
};

char msg[64];

int Lla_init(struct lla_head **self, int *array);
int Lla_add(struct lla_head *head, int index);
int Lla_remove(struct lla_head *head, int index);
void Lla_display(struct lla_head *head, char *msg);


static inline void ADD(struct lla_head *head, int i)
{
	sprintf(msg, "Add %d", i);
	Lla_add(head, i);
	Lla_display(head, msg);
}

static inline void REMOVE(struct lla_head *head, int i)
{
	sprintf(msg, "Remove %d", i);
	Lla_remove(head, i);
	Lla_display(head, msg);
}
#endif
