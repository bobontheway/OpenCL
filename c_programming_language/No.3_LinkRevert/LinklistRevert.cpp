#include <stdio.h>
#include <stdlib.h>

typedef struct node node_t;

struct node {
	int item;
	node_t *next;
};

node_t *revert(node_t *head)
{
	node_t *t, *r = NULL, *p = head;

	while (p) {
		t = p;
		p = p->next;

		t->next = r;
		r = t;
	}

	return r;
}

void print_list(node_t *h)
{
	node_t *t = h;

	while (t) {
		printf("%d  ", t->item);
		t= t->next;
	}
	printf("\n");
}

int main()
{
	node_t *h = NULL;

	// 分配内存
	for (int i = 0; i < 10; i++) {
		node_t *n = (node_t *)malloc(sizeof(struct node));
		if (n == NULL)
			return EXIT_FAILURE;
		n->item = i;
		n->next = h;
		h = n;
	}

	print_list(h);

	// 单链表逆序
	node_t *r = revert(h);

	print_list(r);

	// 释放动态分配的内存
	node_t *d = r;
	while (d) {
		node_t *t = d;
		d = d->next;
		free(t);
	}
	
	print_list(r);

	return 0;
}
