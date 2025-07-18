/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 04:46:57 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/15 15:06:20 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*sh_op(t_op_shell op, t_shell *shell)
{
	static t_shell	*inner_shell;

	if (op == SH_SET)
		inner_shell = shell;
	else if (op == SH_GET)
		return (inner_shell);
	else if (op == SH_DEL)
	{
		free_shell(&inner_shell);
		inner_shell = NULL;
	}
	return (inner_shell);
}
