#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "hash.h"
/*
uint64_t hash(key_tt buf)
{
	
	//assert(buf);
	uint64_t hash = 0;
	for (int i = 0; buf[i]; i++)
		hash = hash * 33 + buf[i];
	return hash;
	 
	asm(".intel_syntax noprefix\n\t"
		"mov rdi, %0\n\t"
		"xor rcx, rcx\n\t"
		"mov cl, byte [rdi]\n\t"
		"test cl, cl\n\t"
		"je HEXIT\n\t"
		"add rdi, 1\n\t"
		"xor rax, rax\n\t"
		"HAGAIN:\n\t"
		"mov rbx, rax\n\t"
		"add rdi, 1\n\t"
		"shl rbx, 5\n\t"
		"add rax, rbx\n\t"
		"add rax, rcx\n\t"
		"mov cl, byte [rdi - 1]\n\t"
		"test cl, cl\n\t"
		"jne HAGAIN\n\t"
		"jmp HEXIT2\n\t"
		"HEXIT:\n\t"
		"xor rax, rax\n\t"
		"HEXIT2:\n\t"
		".att_syntax noprefix\n\t"
		: "=r"(buf)
		:
		: "rdi", "rcx", "rbx"
	);
	
}
*/
/*
//jenkins one at a time hash
uint64_t hash(key_tt buf)
{
	
	uint64_t hash = 0;
    for(int i = 0; buf[i]; i++)
    {
        hash += buf[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
    
	asm(".intel_syntax noprefix\n\t"
		"mov rdi, %0\n\t"
		"xor rax, rax\n\t"
		"xor rcx, rcx\n\t"
		"mov al, byte [rdi]\n\t"
		"test al, al\n\t"
		"je HEXIT\n"
		"HAGAIN:\n\t"
		"inc rdi\n\t"
		"add rax, rcx\n\t"
		"mov rcx, rax\n\t"
		"shl rcx, 10\n\t"
		"add rax, rcx\n\t"
		"mov rcx, rax\n\t"
		"shr rcx, 6\n\t"
		"xor rcx, rax\n\t"
		"xor rax, rax\n\t"
		"mov al, byte [rdi - 1]\n\t"
		"test al, al\n\t"
		"jne HAGAIN\n"
		"HEXIT:\n\t"
		"lea rcx, [rcx + rcx * 8]\n\t"
		"add rax, rcx\n\t"
		"shr rax, 11\n\t"
		"xor rax, rcx\n\t"
		"mov rcx, rax\n\t"
		"shl rcx, 15\n\t"
		"add rax, rcx\n\t"
		".att_syntax noprefix\n\t"
		: "=r"(buf)
		:
		: "rdi", "rcx"
	);
		
} */

//CRC8
uint64_t hash(key_tt buf)
{
	/*
	uint64_t hash = 0xFF;
	for (int i = 0; buf[i]; i++) {
		hash ^= buf[i];
		for (int j = 0; j < 8; j++)
			hash = hash & 0x80 ? (hash << 1) ^ 0x31 : hash << 1;
	}
	return hash;
	*/
	asm(".intel_syntax noprefix\n\t"
		"mov rdi, %0\n\t"
		"mov rax, 0xFF\n\t"
		"xor rcx, rcx\n\t"
		"HAGAIN:\n\t"
		"mov bl, byte [rdi]\n\t"
		"test bl, bl\n\t"
		"je HEXIT\n\t"
		"mov rcx, 8\n\t"
		"inc rdi\n\t"
		"xor rax, rbx\n\t"
		"HLOOP:\n\t"
		"shl rax, 1\n\t"
		"test rax, 0x80\n\t"
		"jne CRC\n\t"
		"xor rax, 0x31\n\t"
		"CRC:\n\t"
		"loop HLOOP\n\t"
		"jmp HAGAIN\n\t"
		"HEXIT:\n\t"
		".att_syntax noprefix\n\t"
		: "=r"(buf)
		:
		: "rdi", "rcx", "rbx"
	);
}

uint8_t hashkeycheck(key_tt key1, key_tt key2)
{
	return strcmp(key1, key2);
}
struct Hashtable* htable_ctor(size_t size)
{
	struct Hashtable* htable = calloc(1, sizeof(struct Hashtable));
	htable->size = size;
	htable->hnodearray = calloc(size, sizeof(struct Hashnode*));
	return htable;
}
void htable_dtor(struct Hashtable* htable)
{
	assert(htable);
	for (int i = 0; i < htable->size; i++) {
		while(htable->hnodearray[i]) {
			struct Hashnode* hnodetmp = htable->hnodearray[i];
			htable->hnodearray[i] = htable->hnodearray[i]->next;
			free(hnodetmp);
		}	
	}
	free(htable->hnodearray);
	htable->size = 0;
	free(htable);
}
void htable_dump(struct Hashtable* htable) 
{
	printf("Hashtable dump:\n");
	struct Hashnode *hnodetmp = NULL;
	for (int i = 0; i < htable->size; i++) {
		hnodetmp = htable->hnodearray[i];
		size_t length = 0;
		while(hnodetmp) {
			length++;
			//printf("[%s]: %u\n", hnodetmp->key, hnodetmp->value);
			hnodetmp = hnodetmp->next;
		}
		printf("%d %lu\n", i, length);
	}
}
struct Hashnode* hnode_create(key_tt key, value_t value, struct Hashnode* next) 
{
	struct Hashnode* hashptr = calloc(1, sizeof(struct Hashnode));
	hashptr->key = key;
	hashptr->value = value;
	hashptr->next = next;
	return hashptr;
}
struct Hashnode* hnode_search(struct Hashtable* htable, key_tt key)
{
	assert(htable);
	struct Hashnode* hnode = htable->hnodearray[hash(key) % htable->size];
	while (hnode) {
		if (!hashkeycheck(hnode->key, key))
			return hnode;
		hnode = hnode->next;
	}
	return NULL;
}
void hnode_insert(struct Hashtable* htable, key_tt key, value_t value)
{
	assert(htable);
	struct Hashnode* hnode = hnode_search(htable, key);
	if (hnode) {
		hnode->value++;
	} else {
		size_t index = hash(key) % htable->size;
		hnode = hnode_create(key, value, htable->hnodearray[index]);
		assert(hnode);
		htable->hnodearray[index] = hnode;
	}
}
void hnode_delete(struct Hashtable* htable, key_tt key)
{
	size_t index = hash(key) % htable->size;
	struct Hashnode* hnode = htable->hnodearray[index];
	if(!hnode)
		return;
	if (!hashkeycheck(hnode->key, key)) {
		htable->hnodearray[index] = hnode->next;
		free(hnode);
		return;
	}
	while (hnode->next) {
		if (!hashkeycheck(hnode->next->key, key)) {
			struct Hashnode* hnodetmp = hnode->next;
			hnode->next = hnode->next->next;
			free(hnodetmp);
			return;
		}
		hnode = hnode->next;
	}
}
struct Hashnode* hnode_maxvalue(struct Hashtable* htable)
{
	assert(htable);
	struct Hashnode* hnodemax = NULL;
	for (size_t i = 0; i < htable->size; i++) {
		struct Hashnode* hnode = htable->hnodearray[i];
		if (!hnodemax)
			hnodemax = hnode;
		while (hnode) {
			if (hnode->value > hnodemax->value)
				hnodemax = hnode;
			hnode = hnode->next;
		}
	}
	return hnodemax;
}