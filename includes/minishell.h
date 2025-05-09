#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
// debug用
# include <stdbool.h>

// 字句解析
typedef enum e_token_type
{
	TK_RESERVED,
	TK_WORD,
	TK_EOF
}					t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

// 構文解析
typedef enum e_node_kind
{
	ND_PIPE, // "|"単体
	ND_CMD,  // その他のコマンドとリダイレクト記号のまとまり
}					t_node_kind;

typedef enum s_redir_kind
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_kind;

typedef struct s_redir
{
	t_redir_kind	kind;
	char			*str;
}					t_redir;

typedef struct s_node
{
	t_node_kind		kind;
	char			*value;
	struct s_node	*lhs;
	struct s_node	*rhs;
	// コマンドとオプション、引数
	char			**argv;
	int				argc;
	// リダイレクト
	t_redir			**redirs;
	int				redir_count;
}					t_node;

// minishell全体
typedef struct s_shell
{
	int				status;
	// int				last_status;
	char			**envp_cp;
	t_token			*tokens;
	t_node			*ast;
}					t_shell;

// init
// init.c
int					init(t_shell **shell, char **envp);

// execution
// child_exec.c
void				exec_if_relative_path(char **cmds, char **envp);
void				exec_if_absolute_path(char **cmds, char **envp);
void				execute_in_child(char **cmds, char **envp);

// execute.c
int					execute(char **cmds, char **envp);

// find_path.c
char				*search_path(char *cmd_name, char **path_list,
						char *path_tail);
char				*resolve_cmd_path(char *cmd, char *path_env);

// tokenization
// tokenize.c
int					tokenize(char *line, t_token **tokens);
int					is_single_metachar(char *p);
int					is_two_metachar(char *p);

// parsing
// parse.c
int					parse(t_token *tokens, t_node **ast);
// new_node.c
t_node				*new_pipe_node(t_node *lhs, t_node *rhs);
t_node				*new_command_node(void);
// peek.c
int					peek_word(t_token *token);
int					peek_redir_op(t_token *token);
// consume.c
int					consume_word(t_token **rest, char **redir_str);
int					consume_reserved(t_token **rest, char *op);
// expansion
// expand.c
int					expand(t_node *node, char **envp);
char				*append_string_free(char *dst, char *src);

// utils
// ft_getenv.c
char				*ft_getenv(char *name, char **envp);
// xrealloc.c
// void				*xrealloc(void *ptr, size_t old_size, size_t new_size);

// free
// free.c
void				free_2d_array(char **array);
void				free_tokens(t_token *token);
void				free_redirs(t_redir **redirs);
void				free_ast(t_node *ast);
void				free_shell(t_shell *shell);

// debug
// debug_tokenize.c
void				print_tokens(t_token *token);
void				debug_tokenizer(t_token *tokens);
void				print_indent(int depth, bool is_last);
void				print_redir(t_redir *redir, int depth);
void				print_ast(t_node *node, int depth);
void				debug_parser(t_node *ast);
void				debug_expand(t_node *ast);

#endif