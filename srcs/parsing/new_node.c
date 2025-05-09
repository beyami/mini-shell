#include "minishell.h"

// 新しいパイプノードを作成して左右のノードを接続
t_node	*new_pipe_node(t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
	{
		free_ast(lhs);
		free_ast(rhs);
		return (NULL);
	}
	node->kind = ND_PIPE;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

// 新しいコマンドノードを作成
t_node	*new_command_node(void)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = ND_CMD;
	node->argv = NULL;
	node->argc = 0;
	node->redirs = NULL;
	node->redir_count = 0;
	return (node);
}
