#include "minishell.h"

// SIGINTシグナルハンドラ
// 新しい行で新しいプロンプトを表示 & グローバル変数の更新
void	handle_sigint(int signum)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_sig_received = signum;
}

// SIGINTとSIGQUITに対してシグナルハンドラなどの設定
void	set_sigs_handler(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}
