#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <stdlib.h>

#define NR_WP 32 //�����˹۲����������Ϊ 32
// wp_pool��һ�� WP �ṹ�����飬�����洢���еĹ۲�㡣
// head��ָ��ǰ�����ͷ��������ǰ���л�Ĺ۲�㡣
// free_��ָ����й۲�������ͷ����
static WP wp_pool[NR_WP];
static WP *head, *free_;

//��ʼ���۲���
// init_wp_pool����ʼ���۲��ء���ÿ�� wp_pool �еĹ۲�㰴˳�������������γ�һ����������head ��ʼ��Ϊ NULL��free_ ָ���һ�����еĹ۲�㡣
void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;
	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
//new_WP���ӿ��������з���һ���µĹ۲�㡣��� free_ ��Ϊ�գ��򷵻�һ�����еĹ۲�㣬������ free_ ָ�롣
static WP* new_WP() {
	assert(free_ != NULL);
	WP *p = free_;
	free_ = free_->next;
	return p;
}
//free_WP�����۲�� p �ͷŻؿ������������ͷŹ۲���б���ı��ʽ�ڴ档
static void free_WP(WP *p) {
	assert(p >= wp_pool && p < wp_pool + NR_WP);
	free(p->expr);
	p->next = free_;
	free_ = p;
}
//set_watchpoint������һ���µĹ۲�㡣�������ʽ e������ɹ����򴴽�һ���µĹ۲�㣬��������뵽�۲�������ͷ�������ع۲��ı�š�
int set_watchpoint(char *e) {
	uint32_t val;
	bool success;
	val = expr(e, &success);
	if(!success) return -1;

	WP *p = new_WP();
	p->expr = strdup(e);
	p->old_val = val;

	p->next = head;
	head = p;

	return p->NO;
}
//delete_watchpoint��ɾ��ָ����ŵĹ۲�㡣����ҵ��ù۲�㣬������������Ƴ������ͷ����ڴ档
bool delete_watchpoint(int NO) {
	WP *p, *prev = NULL;
	for(p = head; p != NULL; prev = p, p = p->next) {
		if(p->NO == NO) { break; }
	}

	if(p == NULL) { return false; }
	if(prev == NULL) { head = p->next; }
	else { prev->next = p->next; }

	free_WP(p);
	return true;
}
//list_watchpoint���г����е�ǰ���õĹ۲�㡣���û�й۲�㣬�������û�й۲�㡱�����򣬴�ӡÿ���۲��ı�š����ʽ�͵�ǰֵ��
void list_watchpoint() {
	if(head == NULL) {
		printf("No watchpoints\n");
		return;
	}

	printf("%8s\t%8s\t%8s\n", "NO", "Address", "Enable");
	WP *p;
	for(p = head; p != NULL; p = p->next) {
		printf("%8d\t%s\t%#08x\n", p->NO, p->expr, p->old_val);
	}
}
//scan_watchpoint��ɨ�����й۲�㣬�����ֵ�Ƿ����仯���������ĳ���۲���ֵ�����˱仯���򷵻ظù۲�㣻���򣬷��� NULL��
WP* scan_watchpoint() {
	WP *p;
	for(p = head; p != NULL; p = p->next) {
		bool success;
		p->new_val = expr(p->expr, &success);
		if(p->old_val != p->new_val) {
			return p;
		}
	}

	return NULL;
}

