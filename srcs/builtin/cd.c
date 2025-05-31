/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 22:28:01 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*move_to_env(char *val_name)
{
	int		res;
	t_env	*env;

	env = (t_env *)ft_env(ENV_GET_STRUCT, val_name);
	if (!env)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s not set\n", val_name);
		return (NULL);
	}
	if (access(env->val, X_OK) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd:"
			" %s: Permission denied\n", env->val);
		return (NULL);
	}
	res = chdir(env->val);
	if (res != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s\n", strerror(errno));
		return (NULL);
	}
	return (ft_strdup(env->val));
}

char	*update_envs(char *path)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*cp_path;
	char	*tmp;

	if (!path)
		return (NULL);
	cp_path = ft_strdup(path);
	if (!cp_path)
		return (NULL);
	old_pwd = (t_env *)ft_env(ENV_GET_STRUCT, "OLDPWD");
	pwd = (t_env *)ft_env(ENV_GET_STRUCT, "PWD");
	if (pwd && old_pwd)
	{
		tmp = ft_strjoin("OLDPWD=", pwd->val);
		if (!tmp || ft_env(ENV_SET, tmp) == NULL)
		{
			free(tmp);
			inner_exit(1);
			return (NULL);
		}
		free(tmp);
	}
	if (pwd)
	{
		tmp = ft_strjoin("PWD=", path);
		if (!tmp || ft_env(ENV_SET, tmp) == NULL)
		{
			free(tmp);
			inner_exit(1);
		}
		free(tmp);
	}
	return (ft_cwd(PWD_SET, cp_path));
}

int	cd(int argc, char **argv)
{
	char	*path;

	if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		path = move_to_env("HOME");
	else if (ft_strncmp("-", argv[1], 2) == 0)
		path = move_to_env("OLDPWD");
	else
		path = move_to_some(argv[1]);
	update_envs(path);
	free(path);
	return (EXIT_SUCCESS);
}
