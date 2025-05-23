#include "minishell.h"

int	count_valid_argv(char ***argv)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

void	copy_valid_args(char ***argv, char **new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i][0] != '\0')
		{
			new[j] = (*argv)[i];
			j++;
		}
		else
			free((*argv)[i]);
		i++;
	}
}

// argvから空文字列を削除
int	rm_empty_argv_str(int *argc, char ***argv)
{
	int		valid_cnt;
	char	**new;

	valid_cnt = count_valid_argv(argv);
	*argc = valid_cnt;
	new = (char **)malloc(sizeof(char *) * (valid_cnt + 1));
	if (!new)
	{
		perror("minishell: malloc");
		return (1);
	}
	copy_valid_args(argv, new);
	new[valid_cnt] = NULL;
	free((*argv));
	if (valid_cnt == 0)
	{
		free(new);
		*argv = NULL;
	}
	else
		*argv = new;
	return (0);
}
