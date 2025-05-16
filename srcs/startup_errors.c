#include "minishell.h"

void	handle_startup_errors(int argc, char **argv)
{
	(void)argv;
	if (argc > 1)
	{
		write(STDERR_FILENO, "minishell: too many arguments\n", 30);
		exit(1);
	}
	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
	{
		write(STDERR_FILENO, "minishell: stdin is not a tty\n", 30);
		exit(1);
	}
}
