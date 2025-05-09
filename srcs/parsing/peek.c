#include "minishell.h"

// トークンがTK_WORDかどうか確認
int	peek_word(t_token *token)
{
	if (token->type == TK_WORD)
		return (1);
	return (0);
}

// トークンがリダイレクト演算子かどうか確認
int	peek_redir_op(t_token *token)
{
	// トークンが予約語でなければエラー
	if (token->type != TK_RESERVED)
		return (0);
	// トークンがリダイレクト記号かどうか
	if (ft_strncmp(token->str, ">>", 3) == 0 || ft_strncmp(token->str, ">",
			2) == 0 || ft_strncmp(token->str, "<<", 3) == 0
		|| ft_strncmp(token->str, "<", 2) == 0)
		return (1);
	return (0);
}
