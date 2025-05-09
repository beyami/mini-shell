#include "minishell.h"

char	**get_envp_copy(char **envp)
{
	char	**cp;
	int		env_count;
	int		i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	cp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!cp)
	{
		perror("minishell");
		return (NULL);
	}
	i = 0;
	while (i < env_count)
	{
		cp[i] = ft_strdup(envp[i]);
		if (!cp[i])
		{
			while (--i >= 0)
				free(cp[i]);
			free(cp);
			return (NULL);
		}
		i++;
	}
	cp[i] = NULL;
	return (cp);
}

int	init(t_shell **shell, char **envp)
{
	(*shell) = (t_shell *)malloc(sizeof(t_shell));
	if (!(*shell))
	{
		perror("minishell");
		return (-1);
	}
	(*shell)->status = 0;
	// TODO: 直前のexitステータスを扱うように変更
	// (*shell)->last_status = 0;
	(*shell)->tokens = NULL;
	if (!envp)
		(*shell)->envp_cp = NULL;
	else
	{
		(*shell)->envp_cp = get_envp_copy(envp);
		if (!(*shell)->envp_cp)
		{
			free(*shell);
			return (-1);
		}
	}
    (*shell)->ast = NULL;
	return (0);
}
