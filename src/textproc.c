#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "hash.h"
#include "textproc.h"

void text_read(struct Hashtable* htable, char* text)
{
	assert(text);
	assert(htable);
	text_proc(text);
	size_t ofs = 0;
	while (text[ofs]) {
		size_t locofs = 0;
		while(!isalnum(text[ofs]))
			ofs++;
		while (isalnum(text[ofs + locofs]))
			locofs++;
		text[ofs + locofs] = '\0';
		hnode_insert(htable, text + ofs, 1);
		ofs = ofs + locofs + 1;
	}
}
void text_proc(char* text)
{
	for (size_t i = 0; text[i]; i++) {
		switch (text[i]) {
		case '.':
		case ',':
		case ';':
		case ':':
		case '(':
		case ')':
		case '[':
		case ']':
		case '!':
		case '?':
		case '\'':
		case '\n':
		case '\t':
			text[i] = ' ';	
			break;	
		default:
			break;
		}
	}
}