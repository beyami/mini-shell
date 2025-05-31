#include "minishell.h"

// SIGINTとSIGQUITに対してシグナルハンドラなどの設定
void	init_signals(void)
{
	set_main_sigint();
	set_main_sigquit();
}

char	**get_envp_copy(char **envp)
{
	char	**cp;
	int		env_count;
	int		i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	cp = (char **)ft_malloc(sizeof(char *) * (env_count + 1));
	if (!cp)
		return (NULL);
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

int	init_env(char **envp)
{
	t_env	*head;
	int		i;

	i = 0;
	while (envp[i])
	{
		head = (t_env *)ft_env(ENV_ADD, envp[i]);
		if (!head)
		{
			ft_env(ENV_DEL_ALL, NULL);
			return (1);
		}
		++i;
	}
	return (0);
}

int	init_pwd(void)
{
	char	*pwd;

	pwd = ft_env(ENV_GET_VAL, "PWD");
	if (!pwd)
	{
		pwd = (char *)ft_malloc(sizeof(char) * (PATH_MAX + 1));
		if (!pwd)
			return (1);
		getcwd(pwd, PATH_MAX);
	}
	else
	{
		pwd = ft_strdup(pwd);
		if (!pwd)
		{
			return (1);
		}
	}
	ft_cwd(PWD_SET, pwd);
	return (0);
}

int	pure_atoi(char *num)
{
	int	rtn;
	int	i;
	int	sign;

	rtn = 0;
	i = 0;
	sign = 1;
	if (ft_isspace(num[i]))
		++i;
	if (num[i] == '-')
	{
		sign = -1;
		++i;
	}
	else if (num[i] == '+')
		++i;
	while (ft_isdigit(num[i]))
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	return (rtn * sign);
}

int	is_valid_shlvl(char *num)
{
	int	i;

	i = 0;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '+' || num[i] == '-')
		++i;
	while (ft_isdigit(num[i]))
		++i;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '\0')
		return (0);
	return (1);
}

int	increment_shlvl(int shlvl)
{
	char	*str_shlvl;
	char	*env_shlvl;

	str_shlvl = ft_itoa(shlvl + 1);
	env_shlvl = ft_strjoin("SHLVL=", str_shlvl);
	if (!str_shlvl || !env_shlvl)
	{
		free(str_shlvl);
		free(env_shlvl);
		ft_env(ENV_DEL_ALL, NULL);
		return (1);
	}
	if (ft_env(ENV_SET, env_shlvl) == NULL)
	{
		free(str_shlvl);
		free(env_shlvl);
		ft_env(ENV_DEL_ALL, NULL);
		return (1);
	}
	free(str_shlvl);
	free(env_shlvl);
	return (0);
}

int	init_shlvl(void)
{
	char	*shlvl;
	int		lvl;

	shlvl = ft_env(ENV_GET_VAL, "SHLVL");
	if (!shlvl || is_valid_shlvl(shlvl))
	{
		if (ft_env(ENV_ADD, "SHLVL=1") == NULL)
			return (1);
		return (0);
	}
	lvl = pure_atoi(shlvl);
	if (lvl > 998)
	{
		ft_dprintf(STDERR_FILENO, "minishell: warning: shell level (%d) "
			"too high, resetting to 1\n", lvl + 1);
		if (ft_env(ENV_SET, "SHLVL=1") == NULL)
			return (1);
	}
	else if (lvl < 0)
	{
		if (ft_env(ENV_SET, "SHLVL=0") == NULL)
			return (1);
	}
	else if (increment_shlvl(lvl))
		return (1);
	return (0);
}

int	init(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)ft_malloc(sizeof(t_shell));
	if (!shell)
		return (-1);
	sh_op(SH_SET, shell);
	sh_stat(ST_SET, 0);
	shell->tokens = NULL;
	if (init_env(envp))
	{
		free(shell);
		return (-1);
	}
	if (init_pwd())
	{
		free(shell);
		ft_env(ENV_DEL_ALL, NULL);
		return (-1);
	}
	if (init_shlvl())
		inner_exit(1);
	shell->ast = NULL;
	return (0);
}
