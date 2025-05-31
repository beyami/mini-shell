#include "minishell.h"

// execve用のSIGQUIT設定
void	set_exec_sigquit(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		exit(1);
	}
}

// メインプロセスのSIGQUIT設定
void	set_main_sigquit(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		exit(1);
	}
}
