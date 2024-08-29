#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "cpu/reg.h"
#define NR_WP 32
//extern struct sg_header;
//int result;
static WP wp_pool[NR_WP];
static WP *head, *free_;
//extern  CPU_state cpu; //���ö���
//��ʼ���������нڵ㣨ȫ���NULL��
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
//ʵ�ּ��ӵ�صĹ���
//�ú������ڴӴ������е�free_������һ�����õļ��ӵ�ṹ��
//�����������һ����head����Ϊ��ʱ��ֱ��head = temp������Ļ�Ҫ����һ��������������head���һ���ڵ㣬����β�巨�����õĽڵ���ϡ�
WP* new_wp(){
	WP *temp;
	temp = free_;
	free_ = free_->next;
	temp->next = NULL;
	if (head == NULL){
		head = temp;
	} else {
		WP* temp2;
		temp2 = head;
		while (temp2->next != NULL){
			temp2 = temp2->next;
		}
		temp2->next = temp;
	}
	return temp;
}
//��дһ��free_wp()����wp�黹��free_�����С�
//�������������һ�������ǰ���صĽڵ�Ϊ�գ�ֱ��assert(0)���ڶ��ֵ����head����wp������Ļ�Ҫ��head���ҵ���֮���Ӧ��wp��֮����ͷ�巨��wp�嵽free������wp��������ա�
void free_wp(WP *wp){
	if (wp == NULL){
		assert(0);
	}
	if (wp == head){ 
		head = head->next;
	} else {
		WP* temp = head;
		while (temp != NULL && temp->next != wp){
			temp = temp->next;
		}
		temp->next = temp->next->next;
	}
	wp->next =free_;
	free_ = wp;
	wp->result = 0;
	wp->expr[0] = '\0';
}
//ʵ������GDB�ļ��ӵ㹦�ܡ�
//��дһ��checkWP()�������ú��������жϼ��ӵ��Ƿ񴥷���
//���Ƚ��б��ʽ��ֵ��ÿ��NEMUִ����һ��ָ������������û�����ļ��ӵ㣬��������ͣ�����������ӡ���ӵ㡢��ֵ����ֵ��
bool checkWP(){
	bool check = false;
	bool *success = false;
	WP *temp = head;
	int expr_temp;
	while(temp != NULL){
		expr_temp = expr(temp->expr, success);
		if (expr_temp != temp->result){
			check = true;
			printf ("Hint watchpoint %d at address 0x%08x\n", temp->NO, cpu.eip);
			temp = temp->next;
			continue;
		}
		printf ("Watchpoint %d: %s\n",temp->NO,temp->expr);
		printf ("Old value = %d\n",temp->result);
		printf ("New value = %d\n",expr_temp);
		temp->result = expr_temp;
		temp = temp->next;
	}
	return check;
}
//���
void printf_wp(){
	WP *temp = head;
	if (temp == NULL){
		printf("No watchpoints\n");
	}
	while (temp != NULL){
		printf("Watch point %d: %s\n", temp->NO, temp->expr);
		temp = temp->next;
	}
}
//ɾ��
WP* delete_wp(int p, bool *key){
	WP *temp = head;
	while (temp != NULL && temp->NO != p){
		temp = temp->next;
	}
	if (temp == NULL){
		*key = false;
	}
	return temp;
}





