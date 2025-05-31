/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:38:00 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 02:22:44 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*search_val(t_env *head, char *name, int offset, t_val_type type)
{
	t_env	*tmp;
	int		name_len;
	char	*eq;

	eq = ft_strchr(name, '=');
	if (eq)
		name_len = (int)(eq - name - offset);
	else
		name_len = ft_strlen(name);
	tmp = head;
	while (tmp)
	{
		if (((type == VAL_EX && tmp->type == VAL_EX) || type == VAL_SH)
			&& ft_strncmp(tmp->name, name, name_len) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	*ft_env(t_env_op op, char *str)
{
	static t_env	*head;

	if (op == ENV_GET_VAL || op == ENV_GET_STRUCT)
		return (get_env(head, str, op));
	else if (op == ENV_GET_ALL_EX || op == ENV_GET_ALL_SH)
		return (get_env_all(&head, op));
	else if (op == ENV_ADD)
		return (ft_add_env(&head, str, 0));
	else if (op == ENV_SET)
		return (ft_update_env(&head, str, 0));
	else if (op == ENV_SET_PLUS)
		return (ft_update_env(&head, str, 1));
	else if (op == ENV_DEL)
		return (env_del(&head, str));
	else if (op == ENV_DEL_ALL)
		return (env_del_all(&head));
	return (NULL);
}
