// TODO: すべてのファイルに42ヘッダー入れる

#include "minishell.h"

// TODO: シグナルで終了する際は128 + シグナルの値がexitステータス
sig_atomic_t	g_sig_received;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;

	(void)argv;
	input = NULL;
	handle_startup_errors(argc, argv);
	set_sigs_handler();
	init(&shell, envp);
	render_loop(shell);
	free_shell(shell);
	rl_clear_history();
	exit(0);
}
