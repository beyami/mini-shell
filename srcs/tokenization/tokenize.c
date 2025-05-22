#include "minishell.h"

// トークンを追加（新しいトークンへのポインタを返すことでcurrentを一つ進める）
t_token	*add_token(t_token *cur, t_token_type type, char *start, int len)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
	{
		perror("minishell");
		return (NULL);
	}
	new->str = (char *)ft_calloc(sizeof(char), len + 1);
	if (!new->str)
	{
		perror("minishell");
		free(new);
		return (NULL);
	}
	ft_memcpy(new->str, (void *)start, len);
	new->type = type;
	new->next = NULL;
	cur->next = new;
	return (new);
}

// 入力をトークンに分割
void	tokenize(char *line, t_token **tokens)
{
	t_token head;
	t_token *cur;
	char	*p;
	char	*start;
	char	quote;

	head.next = NULL;
	cur = &head;
	p = line;
	while (*p)
	{
		// 空白文字をスキップ
		if (is_blank(*p))
			p++;
		// 2文字のメタ文字をトークン化
		else if (is_two_metachar(p))
		{
			cur = add_token(cur, TK_RESERVED, p, 2);
			if (!cur)
			{
				free_tokens(head.next);
                sh_stat(ST_SET, 1);
				return ;
			}
			p += 2;
		}
		// 1文字のメタ文字をトークン化
		else if (is_single_metachar(p))
		{
			cur = add_token(cur, TK_RESERVED, p, 1);
			if (!cur)
			{
				free_tokens(head.next);
                sh_stat(ST_SET, 1);
				return ;
			}
			p++;
		}
		// 単語をトークン化
		// クォートの中ではメタ文字と空白文字を無視する
		else
		{
			start = p;
			while (*p && !is_blank(*p) && !is_two_metachar(p)
				&& !is_single_metachar(p))
			{
				if (is_quote(p))
				{
					quote = *p;
					p++;
					while (*p && *p != quote)
						p++;
					if (*p == quote)
						p++;
					else
					{
						write(STDERR_FILENO,
							"minishell: syntax error: unclosed quote\n", 41);
						free_tokens(head.next);
						sh_stat(ST_SET, 2);
                        return ;
					}
				}
				else
					p++;
			}
			cur = add_token(cur, TK_WORD, start, p - start);
			if (!cur)
			{
				free_tokens(head.next);
				sh_stat(ST_SET, 1);
                return ;
			}
		}
	}
	// EOFトークンを追加
	if (!add_token(cur, TK_EOF, p, 0))
	{
		free_tokens(head.next);
		sh_stat(ST_SET, 1);
        return ;
	}
	(*tokens) = head.next;
}
