#include "minishell.h"

sig_atomic_t    g_sig_received = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
	input = NULL;
    set_sigs_handler();
	if (init(&shell, envp) < 0)
		exit(1);
    rl_clear_history();
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
        // TODO: それぞれエラー時の処理を追加
		tokenize(input, &shell->tokens);
        parse(shell->tokens, &shell->ast);
		expand(shell->ast, shell->envp_cp);
		execute(shell->ast, envp);
		// 毎ループ更新されるためfree
		free(input);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	free_shell(shell);
	exit(0);
}
