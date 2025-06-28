#include "../src/pipeline.c"

int	main(void)
{
	char *paths[] = {"/usr/bin/cat", "/usr/bin/cat", "./hello.sh", NULL};
	pipeline(paths, NULL);
	return 0;
}

