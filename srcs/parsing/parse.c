#include "minishell.h"

// トークンを解析してリダイレクトノードを生成
t_redir	*parse_redir(t_token **rest, int *stat)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
	{
		(*stat) = -1;
		return (NULL);
	}
	if (consume_reserved(rest, ">>"))
		redir->kind = REDIR_APPEND;
	else if (consume_reserved(rest, ">"))
		redir->kind = REDIR_OUT;
	else if (consume_reserved(rest, "<<"))
		redir->kind = REDIR_HEREDOC;
	else if (consume_reserved(rest, "<"))
		redir->kind = REDIR_IN;
	else
	{
		ft_dprintf(STDERR_FILENO,
					"minishell: syntax error near unexpected token \
			`%s\n'",
					(*rest)->str);
		free(redir);
		(*stat) = 1;
		return (NULL);
	}
	(*stat) = consume_word(rest, &redir->str);
	if (*stat != 0)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}

// トークンを解析してコマンドノード（パイプ以外の部分をまとめたノード）を生成
t_node	*parse_command(t_token **rest, int *stat)
{
	t_token	*cur;
	t_node	*node;
	char	**argv_tmp;
	t_redir	**redirs_tmp;
	int		i;

	cur = *rest;
	node = new_command_node();
	if (!node)
	{
		(*stat) = -1;
		return (NULL);
	}
	// TK_WORDがある間、argvに追加
	while (cur)
	{
		if (peek_word(cur))
		{
			node->argc++;
			argv_tmp = (char **)malloc(sizeof(char *) * (node->argc + 1));
			if (!argv_tmp)
			{
				perror("minishell");
				free_ast(node);
				(*stat) = -1;
				return (NULL);
			}
			i = 0;
			while (i < node->argc - 1)
			{
				argv_tmp[i] = node->argv[i];
				i++;
			}
			argv_tmp[node->argc - 1] = ft_strdup(cur->str);
			if (!argv_tmp[node->argc - 1])
			{
				free_ast(node);
				free(argv_tmp);
				(*stat) = -1;
				return (NULL);
			}
			argv_tmp[node->argc] = NULL;
			free(node->argv);
			node->argv = argv_tmp;
			cur = cur->next;
		}
		// リダイレクト演算子がある間、リダイレクト記号と次のTK_WORD（ファイル名かheredocのデリミタ）をredirsに追加
		else if (peek_redir_op(cur))
		{
			node->redir_count++;
			redirs_tmp = (t_redir **)malloc(sizeof(t_redir *)
					* (node->redir_count + 1));
			if (!redirs_tmp)
			{
				perror("minishell");
				free_ast(node);
				(*stat) = -1;
				return (NULL);
			}
			i = 0;
			while (i < node->redir_count - 1)
			{
				redirs_tmp[i] = node->redirs[i];
				i++;
			}
			redirs_tmp[node->redir_count - 1] = parse_redir(&cur, stat);
			if (!redirs_tmp[node->redir_count - 1])
			{
				free_ast(node);
				free(redirs_tmp);
				return (NULL);
			}
			redirs_tmp[node->redir_count] = NULL;
			free(node->redirs);
			node->redirs = redirs_tmp;
		}
		else
			break ;
	}
	// コマンドが空でリダイレクトもない場合はエラー
	if (node->argc == 0 && node->redir_count == 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		free_ast(node);
		(*stat) = 1;
		return (NULL);
	}
	*rest = cur;
	return (node);
}

// / トークンを解析してASTを生成
t_node	*parse_line(t_token **rest, int *stat)
{
	t_node	*node;
	t_node	*rhs;

	// 最初のパイプまでを解析
	node = parse_command(rest, stat);
	if (!node)
		return (NULL);
	while (consume_reserved(rest, "|"))
	{
		// 右側のコマンドを（次のパイプまで）解析
		rhs = parse_command(rest, stat);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		// パイプノードを作成し、それを根として左右のノードを接続
		node = new_pipe_node(node, rhs);
		if (!node)
		{
			(*stat) = -1;
			return (NULL);
		}
	}
	return (node);
}

// 構文解析
int	parse(t_token *tokens, t_node **ast)
{
	t_token	*rest;
	int		stat;

	if (tokens->type == TK_EOF)
		return (0);
	stat = 0;
	rest = tokens;
	(*ast) = parse_line(&rest, &stat);
	return (stat);
}
