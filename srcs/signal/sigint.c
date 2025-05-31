#include "minishell.h"

// SIGINTのシグナルハンドラ
void	handle_sigint(int signum)
{
	g_sig_received = signum;
	rl_replace_line("", 0);
	rl_done = 1;
}

// execve用のSIGINT設定
void	set_exec_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		exit(1);
	}
}

// メインプロセスのSIGINT設定
void	set_main_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		exit(1);
	}
}
