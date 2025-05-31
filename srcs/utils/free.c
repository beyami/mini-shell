#include "minishell.h"

void	free_2d_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	while (token)
	{
		tmp = token->next;
		free(token->str);
		free(token);
		token = tmp;
	}
}

void	free_redirs(t_redir **redirs)
{
	int	i;

	if (!redirs)
		return ;
	i = 0;
	while (redirs[i])
	{
		free(redirs[i]->str);
		free(redirs[i]);
		i++;
	}
	free(redirs);
}

void	free_ast(t_node *ast)
{
	if (!ast)
		return ;
	if (ast->lhs)
		free_ast(ast->lhs);
	if (ast->rhs)
		free_ast(ast->rhs);
	if (ast->argv)
		free_2d_array(ast->argv);
	if (ast->redirs)
		free_redirs(ast->redirs);
	free(ast);
}

void	free_shell(t_shell **shell)
{
	if ((*shell)->input)
		free((*shell)->input);
	if ((*shell)->tokens)
		free_tokens((*shell)->tokens);
	if ((*shell)->ast)
		free_ast((*shell)->ast);
	free(*shell);
}

void	free_env(t_env *env)
{
	free(env->name);
	free(env->val);
	free(env->full);
	free(env);
}
