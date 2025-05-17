#include "minishell.h"

// 引用符を判定
int	is_quote(char *p)
{
	if (*p == '\'' || *p == '"')
		return (1);
	return (0);
}
