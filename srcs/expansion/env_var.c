#include "minishell.h"

// $が連続している間展開して文字列を連結
char	*expand_env_var_process(char **s)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (**s)
	{
		if (**s == '$')
		{
			result = process_dollar(s, result);
			if (!result)
				return (NULL);
		}
		else
			break ;
	}
	return (result);
}

// ${変数名}または$?を展開してexpトークンを作成
t_exp_tkn	*expand_env_var(char **s, bool is_quoted)
{
	char		*expanded;
	t_exp_tkn	*token;

	expanded = expand_env_var_process(s);
	if (!expanded)
		return (NULL);
	token = new_exp_token(expanded, true, is_quoted);
	if (!token)
	{
		free(expanded);
		return (NULL);
	}
	return (token);
}
