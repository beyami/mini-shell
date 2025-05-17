#include "minishell.h"

void	render_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*input)
			add_history(input);
		// TODO: それぞれエラーの場合の終了を追加
		tokenize(input, &shell->tokens);
		parse(shell->tokens, &shell->ast);
		expand(shell->ast, shell->envp_cp);
		execute(shell->ast, shell->envp_cp);
		free(input);
		free_except_envp(shell);
	}
}
