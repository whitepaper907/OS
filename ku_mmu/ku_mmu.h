#pragma once
#include <stdio.h>
#include <string.h>

typedef struct PTE {
	unsigned char pfn : 6;
	unsigned char u : 1;
	unsigned char p : 1
} ku_pte;

typedef struct NODE{
	void* next;
	char data; // pmem vpn swap pid?
} node;

typedef struct LinkedList {
	node* start;
	node* tail; 
	unsigned int size = 0;
} list;

typedef struct PCB {
	char pid = '';
	char state; // 1 (runnig) 0 (ready)
	void* pdbr;
	void* next;
} pcb;

typedef struct SPACE
{
	unsigned int size;
	list allocated_list;
	list free_list;
} space;

void* pdbr;
space pmem_space;
space swap_space;
char curr_pid = '';
void* process_table = NULL;

void* init_space(unsigned int size) {
	void* init_space = (space*)malloc(sizeof(space));
	init_space->size = size;

	//freelist init
	for (int i = 0; i < size; i+=4 ) {
		node* new_node = (node*)malloc(sizeof(node));
		if (i == 0) init_space->free_list->start = new_node;
		else init_space->free_list->tail->next = new_node;
		init_space->free_list->tail = new_node;
		init_space->free_list->size++;
	}

	return init_space;
}

void* ku_mmu_init(unsigned int mem_size, unsigned int swap_size) {
	//init pmem space
	pmem_space = init_space(mem_size);
	if (pmem_space == NULL) return 0;

	//init swap space
	swap_space = init_space(swap_size);
	if (swap_space == NULL) return 0;

	return pmem_space;
}

pcb* find_pid(char pid) {
	pcb* find_pcb = process_table;
	while (find_pcb != NULL) {
		if (find_pcb->pid == pid)
			return find_pcb;
		find_pcb = find_pcb->next;
	}
	return NULL;
}

int ku_run_proc(char pid, struct ku_pte** ku_cr3) { // struct ku_pte** -> pte
	
	//find pid
	pcb* new_pcb = find_pcb(pid);
	if (new_pcb == NULL) {
		pcb* new_pcb = (pcb*)malloc(sizeof(pcb));
		new_pcb->pid = pid;
		//find on freelist
		void* free_pdbr;
		if (swap_space->free_list->size == 0) {

		}
		else {
			if (pmem_space->free_list->size == 0) {

			}
			else {
				free_pdbr = pmem_space->free_list->start;

			}
		}
		new_pcb->pdbr = pdbr;//freelist에서 할당받기
	}
	new_pcb->state = 1;

	//find curr_pid
	pcb* curr_pcb = find_pid(curr_pid);
	if (curr_pcb != NULL) { //context switch
		curr_pcb->state = '0'; // running to ready
		//insert new_pcb
		new_pcb->next = curr_pcb->next;
		curr_pcb->next = new_pcb;
		curr_pid = pid;
	}
	else process_table = new_pcb;

	return 0;
}

void swap_out() { //pmem to swap 
	node* out = pmem_space->allocated_list->start;
	pmem_space->allocated_list->start = pmem_space->allocated_list->start->next;
	if (swap_space->free_list->size == 0) {
		node* replace = swap_space->allocated_list->start;
		swap_space->allocated_list->start = swap_space->allocated_list->start->next;
		free(replace);
	}
	swap_space->free_list->tail->next = out;
	swap_space->free_list->size++;
}

void swap_in(char pid) { // swap to pmem
	if (pmem_space->free_list->size == 0) swap_out();


}

int ku_page_fault(char pid, char va) { // return 0: success  -1: fail
	//find pid on pcb
	pcb* find_pcb = process_table;
	while (find_pcb != NULL) {
		if (find_pcb->state == '1')
			break;
		find_pcb = find_pcb->next;
	}

	if (find_pcb->pdbr == NULL) { //no page
		ku_pte* new_pte = (ku_pte*)malloc(ku_pte);
		if (swap_list->free_list->size == 0) {
			//replace
		}

	}
	else {
		if (find_pcb->pdbr->p == 0) { 
			//remove find_pcb pdbr in swap
			node* find_node = swap_space->allocate_list->start;
			while(find_node!=NULL){
				
			}
		}
	}
}
