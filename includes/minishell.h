#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
// debug用
# include <stdbool.h>

// TODO: 頭に"./tmp"をつける
# define HEREDOC_TMP "heredoc_tmp_"

// exitステータス操作
typedef enum e_st_op
{
	ST_GET,
	ST_SET,
}					t_st_op;

typedef enum e_pwd_op
{
	PWD_GET,
	PWD_SET,
}					t_pwd_op;

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
	char			*temp_file;
}					t_redir;

typedef struct s_node
{
	t_node_kind		kind;
	// コマンドとオプション、引数
	int				argc;
	char			**argv;
	// リダイレクト
	int				redir_count;
	t_redir			**redirs;
	struct s_node	*lhs;
	struct s_node	*rhs;
	struct s_node	*next_cmd;
	int				in_fd;
	int				pipefd[2];
}					t_node;

// minishell全体
typedef struct s_shell
{
	char			**envp_cp;
	t_token			*tokens;
	t_node			*ast;
}					t_shell;

// init
int					init(t_shell **shell, char **envp);

// execution
void				exec_if_relative_path(char **cmds, char **envp);
void				exec_if_absolute_path(char **cmds, char **envp);
void				exec_cmd(char **cmds, char **envp);
int					exec_builtin_cmd(t_node *node, char ***envp);
void				process_builtin_direct(t_node *node, char ***envp);
void				link_exec_nodes(t_node *node, t_node *rhs, t_node **first,
						t_node **last);
void				execute(t_node *root, char ***envp);
char				*search_path(char *cmd_name, char **path_list,
						char *path_tail, int *status);
char				*resolve_cmd_path(char *cmd, char *path_env, int *status);
int					is_builtin(char *cmd);
void				prepare_pipe(t_node *node);
void				prepare_pipe_child(t_node *node, int count);
void				prepare_pipe_parent(t_node *node, int count);
int					wait_children(pid_t last_pid);

// redirection
int					handle_all_heredocs(t_node *node);
int					apply_redirs(int redir_count, t_redir **redirs);
void				unlink_all_temp(int redir_count, t_redir **redirs);

// tokenization
void				tokenize(char *line, t_token **tokens);
int					is_single_metachar(char *p);
int					is_two_metachar(char *p);
int					is_single_metachar(char *p);
int					is_two_metachar(char *p);
int					is_quote(char *p);
int					is_space(char c);

// parsing
void				parse(t_token *tokens, t_node **ast);
t_node				*new_pipe_node(t_node *lhs, t_node *rhs);
t_node				*new_command_node(void);
int					peek_word(t_token *token);
int					peek_redir_op(t_token *token);
int					consume_word(t_token **rest, char **redir_str);
int					consume_reserved(t_token **rest, char *op);

// expansion
void				expand(t_node *node, char **envp);
char				*append_string_free(char *dst, char *src);
char				*append_char_free(char *dst, char c);

// bulitin
int					cd(int argc, char **argv, char ***envp);
int					echo(int argc, char **argv);
int					export(int argc, char **argv, char ***envp);
int					pwd(int argc, char **argv);
int					unset(int argc, char **argv, char ***envp);
int					env(int argc, char **argv, char **envp);
int					ft_exit(int argc, char **argv);
int					set_env(char *str, char ***envp, int *len);
char				*move_to_some(char *dest, char ***envp);
char				*move_to_env(char ***envp, char *val_name);
int					print_envs(char **envp, int len);

// utils
char				*ft_getenv(char *name, char **envp);
int					sh_stat(t_st_op op, int val);
char				*ft_strndup(char *s, int len);
char				*ft_union(char **split, char delim);
char				*ft_cwd(t_pwd_op op, char *path);
int					ft_isspace(char c);
int					is_valid_env(char *name);
int					ft_split_len(char **split);


// free
void				free_2d_array(char **array);
void				free_tokens(t_token *token);
void				free_redirs(t_redir **redirs);
void				free_ast(t_node *ast);
void				free_shell(t_shell *shell);

// debug
void				print_tokens(t_token *token);
void				debug_tokenizer(t_token *tokens);
void				print_indent(int depth, bool is_last);
void				print_redir(t_redir *redir, int depth);
void				print_ast(t_node *node, int depth);
void				debug_parser(t_node *ast);
void				debug_expand(t_node *ast);
void				debug_exec_list(t_node *node);

#endif