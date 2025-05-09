#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
	input = NULL;
	if (init(&shell, envp) < 0)
		exit(EXIT_FAILURE);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		// TODO: exitするかどうか & ステータスの管理
		shell->status = tokenize(input, &shell->tokens);
		if (shell->status < 0)
		{
			free(input);
			free_shell(shell);
			exit(EXIT_FAILURE);
		}
		shell->status = expand_tokens(shell->tokens, shell->envp_cp);
		if (shell->status < 0)
		{
			free(input);
			free_shell(shell);
			exit(EXIT_FAILURE);
		}
		// TODO: debugあとで消す
		debug_tokenizer(shell->tokens);
        shell->status = parse(shell->tokens, &shell->ast);
        if (shell->status < 0)
        {
            free(input);
            free_shell(shell);
            exit(EXIT_FAILURE);
        }
        // TODO: debugあとで消す
        debug_parser(shell->ast);
		// execute(tokens, envp);
		// 毎ループ更新されるためfree
		free(input);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	free_shell(shell);
	exit(EXIT_SUCCESS);
}
