#include "minishell.h"

void	handle_startup_errors(int argc, char **argv)
{
	(void)argv;
	if (argc > 1)
	{
		write(STDERR_FILENO, "minishell: too many arguments\n", 30);
		exit(1);
	}
	// 標準入力が端末に接続されている（=インタラクティブシェルとして起動されている）ことを確認
	if (!isatty(STDIN_FILENO))
	{
		write(STDERR_FILENO, "minishell: stdin is not a tty\n", 30);
		exit(1);
	}
	// 標準エラー出力が端末に接続されていることを確認
	if (!isatty(STDERR_FILENO))
	{
		write(STDERR_FILENO, "minishell: stderr is not a tty\n", 31);
		exit(1);
	}
}
