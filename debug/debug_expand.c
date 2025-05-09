#include "minishell.h"

void debug_expand(t_node *ast)
{
	printf("===AST (expanded) ===\n");
	if (!ast)
	{
		printf("AST is NULL\n");
		return ;
	}
	print_ast(ast, 0);
    printf("===AST END===\n");
}