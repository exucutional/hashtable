#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>

typedef const char* key_tt;
typedef uint32_t value_t;

struct Hashnode
{
	key_tt key;
	value_t value;
	struct Hashnode* next;
};
struct Hashtable
{
	size_t size;
	struct Hashnode** hnodearray;
};
struct Hashtable* htable_ctor(size_t size);
void htable_dump(struct Hashtable* htable);
void htable_dtor(struct Hashtable* htable);
struct Hashnode* hnode_create(key_tt key, value_t value, struct Hashnode* next);
struct Hashnode* hnode_search(struct Hashtable* htable, key_tt key);
struct Hashnode* hnode_maxvalue(struct Hashtable* htable);
void hnode_insert(struct Hashtable* htable, key_tt key, value_t value);
void hnode_delete(struct Hashtable* htable, key_tt key);
uint64_t hash(key_tt buf);
uint8_t hashkeycheck(key_tt key1, key_tt key2);
#endif