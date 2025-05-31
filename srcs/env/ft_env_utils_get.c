/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_get.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:45:44 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 03:31:15 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	list_len(t_env *head, t_env_op op)
{
	t_env	*tmp;
	int		len;

	len = 0;
	tmp = head;
	while (tmp)
	{
		if (op == ENV_GET_ALL_SH || (op == ENV_GET_ALL_EX
				&& tmp->type == VAL_EX))
			++len;
		tmp = tmp->next;
	}
	return (len);
}

void	*get_env(t_env *head, char *name, t_env_op op)
{
	t_env	*target;

	target = search_val(head, name, 0, VAL_EX);
	if (!target)
		return (NULL);
	if (op == ENV_GET_VAL)
		return (target->val);
	return (target);
}

void	*get_env_all(t_env **head, t_env_op op)
{
	int		len;
	char	**rtn;
	t_env	*tmp;
	int		i;

	len = list_len(*head, op);
	rtn = (char **)ft_malloc(sizeof(char *) * (len + 1));
	if (!rtn)
	{
		return (NULL);
	}
	tmp = *head;
	i = 0;
	while (tmp)
	{
		if (op == ENV_GET_ALL_SH || (op == ENV_GET_ALL_EX
				&& tmp->type == VAL_EX))
			rtn[i++] = tmp->full;
		tmp = tmp->next;
	}
	rtn[i] = NULL;
	return (rtn);
}
