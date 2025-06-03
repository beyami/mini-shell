#include "minishell.h"

int	wait_children(pid_t last_pid, t_node *first_cmd)
{
	pid_t	wait_ret;
	int		w_status;
	int		status;

	status = 0;
	while (1)
	{
		wait_ret = wait(&w_status);
		if (wait_ret == last_pid)
		{
			if (WIFSIGNALED(w_status))
			{
				status = 128 + WTERMSIG(w_status);
				if (status == 128 + SIGINT)
				{
					g_sig_received = SIGINT;
					write(STDOUT_FILENO, "\n", 1);
				}
				else if (status == 128 + SIGQUIT)
				{
					g_sig_received = SIGQUIT;
					write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
				}
			}
			else
			{
				status = WEXITSTATUS(w_status);
				g_sig_received = 0;
			}
		}
		else if (wait_ret < 0)
		{
			if (errno == ECHILD)
				break ;
		}
	}
	unlink_all_temp(first_cmd);
	return (status);
}
