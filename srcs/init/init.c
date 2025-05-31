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

void	init_shlvl(void)
{
	char	*shlvl;
	int		lvl;
	char	*lvl_str;
    char    *env_str;

	shlvl = ft_env(ENV_GET_VAL, "SHLVL");
	if (!shlvl || is_valid_shlvl(shlvl))
	{
		ft_env(ENV_ADD, "SHLVL=1");
		return ;
	}
	lvl = pure_atoi(shlvl);
	if (lvl > 998)
	{
		ft_dprintf(STDERR_FILENO,
					"minishell: warning: shell level (%d) "
					"too high, resetting to 1\n",
					lvl + 1);
		ft_env(ENV_SET, "SHLVL=1");
	}
	else if (lvl < 0)
		ft_env(ENV_SET, "SHLVL=0");
	else
	{
		lvl_str = ft_itoa(lvl + 1);
		// TODO: ft_itoaのエラーハンドリング？
        env_str = ft_strjoin("SHLVL=", lvl_str);
		ft_env(ENV_SET, env_str);
		free(lvl_str);
        free(env_str);
	}
}

int	init(t_shell **shell, char **envp)
{
	(*shell) = (t_shell *)ft_malloc(sizeof(t_shell));
	if (!(*shell))
		return (-1);
	sh_stat(ST_SET, 0);
	(*shell)->input = NULL;
	(*shell)->tokens = NULL;
	(*shell)->ast = NULL;
	if (init_env(envp))
	{
		free(*shell);
		return (-1);
	}
	if (init_pwd())
	{
		free(*shell);
		ft_env(ENV_DEL_ALL, NULL);
		return (-1);
	}
	init_shlvl();
	return (0);
}
