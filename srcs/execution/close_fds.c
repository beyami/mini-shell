#include "minishell.h"

void	close_fds(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && in_fd != -1)
		close(in_fd);
	if (out_fd != STDOUT_FILENO && out_fd != -1)
		close(out_fd);
}
