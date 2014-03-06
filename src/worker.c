#include "worker.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "result.h"

struct worker_list{
	struct nodenet_worker worker;
	struct worker_list *next;
};

static struct worker_list *_workers = NULL;
static const char *_worker_path = "workers";

int nodenet_worker_create(const char * name)
{
	void * dl = NULL;
	char file_name[128];
	struct worker_list *new_worker_node;

	struct nodenet_worker *result = nodenet_worker_find(name);
	if(result)
		return NODENET_SUCCESS;

#ifdef _WIN32
	sprintf(file_name, "%s/%s.dll", _worker_path, name);
	dl = LoadLibrary(file_name);
#else
	sprintf(file_name, "%s/%s.so", _worker_path, name);
	dl = dlopen(file_name, RTLD_NOW | RTLD_GLOBAL);
#endif

	if (dl == NULL) {
		fprintf(stderr, "try open %s failed\n",name);
		return NODENET_OPEN_LIBRARY_FAILURE;
	}

	new_worker_node = (struct worker_list*)malloc(sizeof(struct worker_list));
	if(new_worker_node) {
#ifdef _WIN32
		FreeLibrary((HMODULE)dl);
#endif
		return NODENET_MEMORY_ALLOCATION_FAILURE;
	}

	new_worker_node->next = _workers;
	_workers = new_worker_node;

	new_worker_node->worker.name = strdup(name);
	new_worker_node->worker.module = dl;

#ifdef _WIN32
	new_worker_node->worker.init = (dl_init)GetProcAddress((HMODULE)dl, "module_init");
	new_worker_node->worker.release = (dl_release)GetProcAddress((HMODULE)dl, "module_release");
#endif

	return NODENET_SUCCESS;
}

struct nodenet_worker * nodenet_worker_find(const char *name)
{
	struct worker_list *curr = _workers;
	while(curr)
	{
		if(strcmp(curr->worker.name, name) == 0)
			return &curr->worker;
		curr = curr->next;
	}
	return NULL;
}

void nodenet_worker_free(const char *name)
{
	struct worker_list *curr = _workers, *prev = NULL;
	while(curr) {
		if(strcmp(curr->worker.name, name) == 0)
			break;
		
		prev = curr;
		curr = curr->next;
	}
	
	if(curr) {
		if(prev)
			prev->next = curr->next;
		else
			_workers = curr->next;

#ifdef _WIN32
		FreeLibrary((HMODULE)curr->worker.module);
#endif
		free((void*)curr->worker.name);
		free(curr);
	}
}