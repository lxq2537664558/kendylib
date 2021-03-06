/*	
    Copyright (C) <2012>  <huangweilook@21cn.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/	
#ifndef _HASH_MAP_H
#define _HASH_MAP_H
#include <stdint.h>
typedef struct hash_map* hash_map_t;

typedef uint64_t (*hash_func)(void*);
typedef int32_t (*hash_key_eq)(void*,void*);
typedef void (*hash_on_remove)(void*,void*);

typedef struct 
{
	void    *data1;
	void    *data2;
}hash_map_iter;


hash_map_t hash_map_create(uint32_t slot_size,uint32_t key_size,uint32_t val_size,hash_func,hash_key_eq,hash_on_remove);
void       hash_map_destroy(hash_map_t*);
int32_t        hash_map_insert(hash_map_t,void *key,void *val);
int32_t        hash_map_remove(hash_map_t,void* key);

int32_t        hash_map_is_vaild_iter(hash_map_iter);
hash_map_iter hash_map_find(hash_map_t,void* key); 
int32_t           hash_map_erase(hash_map_t,hash_map_iter);

void*      hash_map_iter_get_val(hash_map_iter);

void       hash_map_iter_set_val(hash_map_iter,void*);


#ifndef HASH_MAP_INSERT
#define HASH_MAP_INSERT(KEY_TYPE,VAL_TYPE,HASH_MAP,KEY,VAL)\
	({ int32_t ret; KEY_TYPE __key = KEY;VAL_TYPE __val = VAL;\
       do ret = hash_map_insert(HASH_MAP,&__key,&__val);\
       while(0);\
       ret;})		
#endif

#ifndef HASH_MAP_REMOVE
#define HASH_MAP_REMOVE(KEY_TYPE,HASH_MAP,KEY)\
	({ int32_t ret; KEY_TYPE __key = KEY;\
       do ret = hash_map_remove(HASH_MAP,&__key);\
       while(0);\
       ret;})		
#endif

#ifndef HASH_MAP_FIND
#define HASH_MAP_FIND(KEY_TYPE,HASH_MAP,KEY)\
	({ hash_map_iter ret; KEY_TYPE __key = KEY;\
       do ret = hash_map_find(HASH_MAP,&__key);\
       while(0);\
       ret;})		
#endif





#endif