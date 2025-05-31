/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:53:57 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 03:31:36 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_update_env(t_env **head, char *str, int offset)
{
	t_env	*target;
	char	*val;
	char	*tmp;

	target = search_val(*head, str, offset, VAL_SH);
	if (!target)
		return (ft_add_env(head, str, offset));
	val = ft_strchr(str, '=') + 1;
	if (offset && target->type == VAL_EX)
		tmp = rm_quotes(ft_strjoin(target->val, val));
	else
	{
		target->type = VAL_EX;
		tmp = rm_quotes(ft_strdup(val));
	}
	if (!tmp)
		return (NULL);
	free(target->val);
	target->val = tmp;
	tmp = ft_strjoin_delim(target->name, '=', target->val);
	if (!tmp)
		return (NULL);
	free(target->full);
	target->full = tmp;
	return (*head);
}
