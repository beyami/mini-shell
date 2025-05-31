#include "minishell.h"

// 展開しない & クォートのついてない文字列を抜粋してexpトークンを作成
// 合わせてポインタを進める
t_exp_tkn	*extract_literal(char **s, int len, bool is_quoted)
{
	char	*str;

	if (len == 0)
		str = ft_strdup("");
	else
		str = ft_strndup(*s, len);
	if (!str)
		return (NULL);
	*s += len;
	return (new_exp_token(str, false, is_quoted));
}
