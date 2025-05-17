#include "minishell.h"

void	wait_children(t_exec ctx)
{
	int	i;
	int	w_status;

	i = 0;
	while (i < ctx.child_count)
	{
		waitpid(ctx.child_pids[i], &w_status, 0);
		if (WIFEXITED(w_status))
			sh_stat(ST_SET, WEXITSTATUS(w_status));
		else if (WIFSIGNALED(w_status))
			sh_stat(ST_SET, 128 + WTERMSIG(w_status));
		else
			sh_stat(ST_SET, 1);
		i++;
	}
}
