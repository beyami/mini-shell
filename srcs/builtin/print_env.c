/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:15:50 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 21:37:02 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * １つの環境変数を
 * declare -x <NAME>=<VALUE>\n
 * の形で表示する
 */
void	print_env(char *env)
{
	int	i;

	if (ft_strncmp(env, "_=", 2) == 0)
		return ;
	write(STDOUT_FILENO, "declare -x ", 11);
	i = 0;
	while (env[i] && env[i] != '=')
		write(STDOUT_FILENO, &env[i++], 1);
	++i;
	write(STDOUT_FILENO, "=\"", 2);
	write(STDOUT_FILENO, env + i, ft_strlen(env + i));
	write(STDOUT_FILENO, "\"", 1);
	write(STDOUT_FILENO, "\n", 1);
}

void	sort_envs(char **envp)
{
	char	*tmp;
	int		i;
	int		j;
	int		len;

	len = 0;
	while (envp[len])
		++len;
	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			++j;
		}
		++i;
	}
}

int	print_envs(void)
{
	int		i;
	char	**envp;

	envp = (char **)(ft_env(ENV_GET_ALL_EX, NULL));
	if (!envp)
		inner_exit(1);
	sort_envs(envp);
	i = 0;
	while (envp[i])
		print_env(envp[i++]);
	return (EXIT_SUCCESS);
}
