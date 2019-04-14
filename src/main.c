#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "hash.h"
#include "textproc.h"

extern void _mprintf(const char*, ...);

int main()
{
	struct Hashtable* htable = htable_ctor(512);
	FILE* fin = fopen("Hamlet.txt", "r");
	assert(fin);
	fseek(fin, 0, SEEK_END);
	size_t size = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	char *text = calloc(size, sizeof(char));	
	fread(text, sizeof(char), size, fin);
	text_read(htable, text);
	printf("TOP 10 MOST POPULAR WORDS IN HAMLET\n");
	for(int i = 0; i < 10; i++) {
		struct Hashnode* hnode = hnode_maxvalue(htable);
		printf("[%s]: %u\n", hnode->key, hnode->value);
		hnode_delete(htable, hnode->key);
	}
	printf("Hashtable search key 'you' 50000000 times\n");
	clock_t begin = clock();
	for (int i = 0; i < 50000000; i++) {
		hnode_search(htable, "asdasdass");
	}
	clock_t end = clock();
	printf("TIME: %lf sec\n", (double)((end - begin)) / CLOCKS_PER_SEC);
	free(text);
	fclose(fin);	
	htable_dtor(htable);
	return  0;
}