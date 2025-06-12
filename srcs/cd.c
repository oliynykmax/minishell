#include "../incl/minishell.h"

int mini_cd(char **argv, int fd, t_shell *s)
{
	int i;
	char *path;

	(void)fd;
	(void)s;
	i = 0;
	while (argv[i])
		i++;
	if (i > 2)
		return 1;
    if (!argv[1])
    {
        path = getenv("HOME");
        if (!path)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else
    	path = argv[1];
    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }
    return 0;
}
