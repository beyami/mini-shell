#include "minishell.h"

volatile sig_atomic_t	g_sig_received;

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	rl_event_hook = event;
	g_sig_received = 0;
	if (init(envp) < 0)
		exit(1);
	shell = sh_op(SH_GET, NULL);
	while (1)
	{
		init_signals();
		shell->input = readline("minishell$ ");
		if (!shell->input)
			break ;
		if (*shell->input)
			add_history(shell->input);
		interpret(shell);
		finish_loop(shell);
	}
	exit_shell(1);
	return (0);
}
