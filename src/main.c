#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "nodenet.h"


char* trim(char* src)
{
	char* cp = 0;

	if(src == 0)
		return 0;

	if(src[0] == 0)
		return src;

	for (cp = src; isspace(*(unsigned char*)cp); ++cp);

	for (cp = src + strlen(src) - 1; cp != src; --cp)
		if(!isspace(*(unsigned char*)cp))
			break;
	cp[1] = '\0';

	return src;
}

void set_option(const char *option)
{
	char key[64];
	char value[64];

	const char *eq = strchr(option,'=');
	strncpy(key, option, eq - option);
	key[eq - option] = 0;

	strcpy(value, eq+1);
	nodenet_setenv(trim(key),trim(value));
}

void load_configure()
{
	FILE *pf = NULL;
	char tmp[1024];

	const char *config_path = nodenet_getenv("config");
	if(config_path)
		pf = fopen(config_path, "r");

	if(!pf)
		return;

	while(!feof(pf)) {
		fgets(tmp,sizeof(tmp),pf);
		set_option(tmp);
	}

	fclose(pf);
}

int main(int argc, char *argv[])
{
	int i=1;
	for (;i<argc; ++i)
		set_option(argv[i]);

	load_configure();
	nodenet_startup();
	return 0;
}