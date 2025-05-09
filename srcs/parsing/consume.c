#include "minishell.h"

// トークンがTK_WORDかどうか確認して文字列を返し次のトークンに進める
int	consume_word(t_token **rest, char **redir_str)
{
	t_token	*cur;

	cur = *rest;
	if (!peek_word(cur))
	{
		if (cur->type == TK_EOF)
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `newline'\n");
		else
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `%s'\n",
				cur->str);
		(*redir_str) = NULL;
		return (1);
	}
	(*redir_str) = ft_strdup(cur->str);
	if (!(*redir_str))
		return (-1);
	*rest = cur->next;
	return (0);
}

// トークンがTK_RESERVEDかつ指定された記号に一致するか確認して次のトークンに進める
int	consume_reserved(t_token **rest, char *op)
{
	t_token	*cur;

	cur = *rest;
	if (cur->type != TK_RESERVED || ft_strncmp(cur->str, op, ft_strlen(op)
			+ 1) != 0)
		return (0);
	*rest = cur->next;
	return (1);
}
