#include "env.h"
#include "worker.h"

#ifdef _WIN32
#include <windows.h>
#endif

char config_path[256] = "configure";

static void env_add_worker(const char* name)
{
	nodenet_worker_create(name);
}

static void env_set_config(const char* name)
{
	strncpy(config_path, name, sizeof(config_path));
}

static const char *env_get_config()
{
	return config_path;
}

struct nodenet_option {
	const char *name;
	void (*set)(const char *arg);
	const char *(*get)();
};

struct nodenet_option options[] = {
	{"worker", env_add_worker, NULL},
	{"config", env_set_config, env_get_config},
};

static const int option_size = _ARRAYSIZE(options);

void nodenet_setenv(const char *key, const char *value)
{
	int i = 0;
	for(; i<option_size; ++i) {
		if(strcmp(key,options[i].name) == 0) {
			if(options[i].set)
				options[i].set(value);
			break;
		}
	}
}

const char * nodenet_getenv(const char *key)
{
	int i = 0;
	for(; i<option_size; ++i) {
		if(strcmp(key,options[i].name) == 0) {
			if(options[i].get)
				return options[i].get();
			return NULL;
		}
	}

	return NULL;
}
