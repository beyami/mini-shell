/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:09:05 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 21:10:59 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inner_exit(int status)
{
	ft_env(ENV_DEL_ALL, NULL);
	sh_op(SH_DEL, NULL);
	exit(status);
}
