#include "minishell.h"

void	print_indent(int depth, bool is_last)
{
	for (int i = 0; i < depth - 1; i++)
		printf("│   ");
	if (depth > 0)
		printf(is_last ? "└── " : "├── ");
}

void	print_redir(t_redir *redir, int depth)
{
	print_indent(depth, true);
	printf("redir: ");
	switch (redir->kind)
	{
	case REDIR_IN:
		printf("< ");
		break ;
	case REDIR_OUT:
		printf("> ");
		break ;
	case REDIR_APPEND:
		printf(">> ");
		break ;
	case REDIR_HEREDOC:
		printf("<< ");
		break ;
	}
	printf("%s\n", redir->str);
}

void	print_ast(t_node *node, int depth)
{
	if (!node)
		return ;
	if (node->kind == ND_PIPE)
	{
		print_indent(depth, false);
		printf("PIPE\n");
		print_ast(node->lhs, depth + 1);
		print_ast(node->rhs, depth + 1);
	}
	else if (node->kind == ND_CMD)
	{
		print_indent(depth, false);
		if (node->argv)
		{
			printf("CMD: ");
			for (int i = 0; i < node->argc; i++)
				printf("%s ", node->argv[i]);
			printf("\n");
		}
		else
			puts("argv is NULL");
		for (int i = 0; i < node->redir_count; i++)
			print_redir(node->redirs[i], depth + 1);
	}
}

void	debug_parser(t_node *ast)
{
    printf("===AST===\n");
	if (!ast)
	{
		printf("AST is NULL\n");
		return ;
	}
	print_ast(ast, 0);
    printf("===AST END===\n");
}
