#include "minishell.h"

t_token	*free_token(t_token *cur)
{
	t_token	*next;

	next = cur->next;
	free(cur->str);
	free(cur);
	return (next);
}

void	remove_empty_node(t_token **head)
{
	t_token	*prev;
	t_token	*cur;

	prev = NULL;
	cur = *head;
	while (cur)
	{
		if (!cur->str[0] && cur->type != TK_EOF)
		{
			if (!prev)
				*head = cur->next;
			else
				prev->next = cur->next;
			cur = free_token(cur);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}
