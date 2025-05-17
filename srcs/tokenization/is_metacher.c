#include "minishell.h"

// パイプとリダイレクトの記号を判定
int	is_single_metachar(char *p)
{
	if (*p == '|' || *p == '<' || *p == '>')
		return (1);
	return (0);
}

// heredocとappendの記号を判定
int	is_two_metachar(char *p)
{
	if (ft_strncmp(p, "<<", 2) == 0 || ft_strncmp(p, ">>", 2) == 0)
		return (1);
	return (0);
}
