#ifndef NODENET_MODULE_H
#define NODENET_MODULE_H

typedef void *(*dl_init)(void);
typedef void *(*dl_release)(void *inst);

struct nodenet_worker {
	const char *name;
	void *module;
	dl_init init;
	dl_release release;
};

int nodenet_worker_create(const char *name);
void nodenet_worker_free(const char *name);

struct nodenet_worker * nodenet_worker_find(const char *name);

#endif