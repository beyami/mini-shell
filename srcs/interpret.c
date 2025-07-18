/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:14:25 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 19:38:59 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_stage_ret(int ret)
{
	if (ret < 0)
	{
		sh_stat(ST_SET, 1);
		ft_exit(1, NULL, 0);
		return (1);
	}
	else if (ret != 0)
	{
		sh_stat(ST_SET, ret);
		return (ret);
	}
	return (0);
}

void	interpret(t_shell *shell)
{
	int	ret;

	ret = 0;
	ret = tokenize(shell->input, &shell->tokens);
	free(shell->input);
	shell->input = NULL;
	if (handle_stage_ret(ret) != 0)
		return ;
	ret = parse(shell->tokens, &shell->ast);
	if (handle_stage_ret(ret) != 0)
		return ;
	ret = expand(shell->ast);
	if (handle_stage_ret(ret) != 0)
		return ;
	ret = execute(shell->ast);
	if (handle_stage_ret(ret) != 0)
		return ;
}
