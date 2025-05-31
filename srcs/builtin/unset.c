/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:58:27 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/25 10:36:25 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc && argv[i])
	{
		ft_env(ENV_DEL, argv[i]);
		++i;
	}
	return (EXIT_SUCCESS);
}

// char	**copy_env(char **envp)
// {
// 	int		i;
// 	int		len;
// 	char	**rtn;

// 	len = 0;
// 	while (envp[len])
// 		++len;
// 	rtn = (char **)malloc(sizeof(char *) * (len + 1));
// 	i = 0;
// 	while (envp[i])
// 	{
// 		rtn[i] = strdup(envp[i]);
// 		++i;
// 	}
// 	rtn[i] = NULL;
// 	return (rtn);
// }

// int	export(int argc, char **argv, char ***envp);
// int main(int argc, char **argv, char **envp)
// {

// 	char	**cenv = copy_env(envp);
// 	// while (*cenv)
// 	// 	printf("%s\n", *cenv++);
// 	// export(1, argv, &cenv);
// 	unset(argc, argv, &cenv);
// 	export(1, argv, &cenv);
// }
