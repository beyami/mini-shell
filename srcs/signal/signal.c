#include "minishell.h"

void handle_sigint(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sh_sig.status = 1;
}

void set_sigint(void)
{
	struct sigaction	sa_int;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("minishell");
	}
}

void set_sigquit(void)
{
	struct sigaction	sa_quit;

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("minishell");
	}
}
