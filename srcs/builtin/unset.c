/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:58:27 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/06 16:48:30 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i])
	{
		if (ft_isalnum(name[i]) || name[i] == '_')
			++i;
		else
			return (0);
	}
	return (1);
}

// 指定された変数名のindexを返す
// TODO: よそで使ってたら仕様合わせて整理した方が良いかも?
int	ft_getenv(char *name, char ***envp)
{
	int	i;
	int	name_len;

	if (!is_valid_name(name))
		return (-1);
	i = 0;
	name_len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp(name, (*envp)[i], name_len) == 0
			&& (*envp)[i][name_len] == '=')
			return (i);
		++i;
	}
	return (-1);
}

// rm_index番目をfreeしそれ以外はcopyした新しいenvpを作成し代入する
int	rm_env(int rm_idx, char ***envp)
{
	int		env_len;
	char	**new;
	int		i;
	int		j;

	env_len = 0;
	while ((*envp)[env_len])
		++env_len;
	new = (char **)malloc(sizeof(char *) * (env_len));
	if (!new)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	i = 0;
	j = 0;
	while (j < env_len)
	{
		if (j != rm_idx)
		{
			new[i] = (*envp)[j];
			++i;
		}
		else
			free((*envp)[j]);
		++j;
	}
	free(*envp);
	*envp = new;
	return (EXIT_SUCCESS);
}

// 指定された変数が存在しなかった場合, bashでは成功が返るがminishellではexportに合わせエラーとして返す
int	unset(int argc, char **argv, char ***envp)
{
	int	env_idx;
	int	i;
	int	error;

	i = 1;
	error = 0;
	while (i < argc && argv[i])
	{
		env_idx = ft_getenv(argv[1], envp);
		if (env_idx < 0)
		{
			write(STDERR_FILENO, "minishell: unset: `", 19);
			write(STDERR_FILENO, argv[i], strlen(argv[i]));
			write(STDERR_FILENO, "' :not a valid identifier\n", 26);
			error = 1;
		}
		// mallocの失敗は途中で停止する
		else if (rm_env(env_idx, envp) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		++i;
	}
	if (error)
		return (EXIT_FAILURE);
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
