/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:06:23 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 18:00:28 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
// debug
# include <stdbool.h>

extern volatile sig_atomic_t	g_sig_received;

// TODO: 頭に"/tmp"をつける
# define HEREDOC_TMP "heredoc_tmp_"

// exit status operation
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

typedef enum e_env_op
{
	ENV_GET_VAL,
	ENV_GET_STRUCT,
	ENV_GET_ALL_EX,
	ENV_GET_ALL_SH,
	ENV_ADD,
	ENV_SET,
	ENV_SET_PLUS,
	ENV_DEL,
	ENV_DEL_ALL
}					t_env_op;

typedef enum e_val_type
{
	VAL_SH,
	VAL_EX
}					t_val_type;

typedef struct s_env
{
	t_val_type		type;
	char			*name;
	char			*val;
	char			*full;
	struct s_env	*next;
}					t_env;

// tokenize
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

// parsing
typedef enum e_node_kind
{
	ND_PIPE,
	ND_CMD,
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

// 展開
typedef struct s_exp_tkn
{
	char				*str;
	bool				is_expanded;
    bool                is_quoted;
	struct s_exp_tkn 	*next;
}						t_exp_tkn;

typedef struct s_node
{
	t_node_kind		kind;
	int				argc;
    t_exp_tkn       *argv_lst;
	char			**argv;
	int				redir_count;
	t_redir			**redirs;
	struct s_node	*lhs;
	struct s_node	*rhs;
	struct s_node	*next_cmd;
	int				in_fd;
	int				pipefd[2];
}					t_node;

typedef enum e_op_shell
{
	SH_SET,
	SH_GET,
	SH_DEL
}					t_op_shell;

// minishell
typedef struct s_shell
{
	char			*input;
	t_env			*env_list;
	t_token			*tokens;
	t_node			*ast;
}					t_shell;

// init
int					init(char **envp);
void				init_signals(void);
t_shell				*sh_op(t_op_shell op, t_shell *shell);

// execution
void				exec_if_relative_path(char **cmds, char **envp);
void				exec_if_absolute_path(char **cmds, char **envp);
void				exec_cmd(char **cmds, char **envp);
int					exec_builtin_cmd(t_node *node);
int					process_builtin_direct(t_node *node);
void				link_exec_nodes(t_node *node, t_node *rhs, t_node **first,
						t_node **last);
int					execute(t_node *root);
char				*search_path(char *cmd_name, char **path_list,
						char *path_tail, int *status);
char				*resolve_cmd_path(char *cmd, char *path_env, int *status);
int					is_builtin(char *cmd);
int					prepare_pipe(t_node *node);
void				prepare_pipe_child(t_node *node, int count);
void				prepare_pipe_parent(t_node *node, int count);
int					wait_children(pid_t last_pid);

// redirection
int					handle_all_heredocs(t_node *node);
int					apply_redirs(int redir_count, t_redir **redirs);
void				unlink_all_temp(int redir_count, t_redir **redirs);

// tokenization
int					tokenize(char *line, t_token **tokens);
int					is_single_metachar(char *p);
int					is_two_metachar(char *p);
int					is_single_metachar(char *p);
int					is_two_metachar(char *p);
int					is_quote(char *p);
int					is_blank(char c);

// parsing
int					parse(t_token *tokens, t_node **ast);
t_node				*new_pipe_node(t_node *lhs, t_node *rhs);
t_node				*new_command_node(void);
int					peek_word(t_token *token);
int					peek_redir_op(t_token *token);
int					consume_word(t_token **rest, char **redir_str);
int					consume_reserved(t_token **rest, char *op);

// expansion
int					expand(t_node *node);
int					expand_cmds(t_node *node);
int					expand_redirs(t_node *node);
int					tokenize_with_expansion(t_exp_tkn **head, char *str,
						int env_flag);
int					merge_expansion_tokens(t_exp_tkn **head);
int					split_exp_tokens(t_exp_tkn **head);
t_exp_tkn	        *extract_literal(char **s, int len, bool is_quoted);
t_exp_tkn	        *expand_env_var(char **s, bool is_quoted);
int	update_args_from_exp(t_exp_tkn *head, t_node *node);
t_exp_tkn	        *new_exp_token(char *str, bool is_expanded, bool is_quoted);
void				append_exp_token(t_exp_tkn **head, t_exp_tkn *new);
void				free_exp_tokens(t_exp_tkn *head);

// bulitin
int					cd(int argc, char **argv);
int					echo(int argc, char **argv);
int					export(int argc, char **argv);
int					pwd(int argc, char **argv);
int					unset(int argc, char **argv);
int					env(int argc, char **argv);
int					ft_exit(int argc, char **argv, int print);
char				*move_to_some(char *dest);
char				*move_to_env(char *val_name);
int					print_envs(void);
void				inner_exit(int status);
int					check_access(char *dest, char *path);

// signal
void				set_main_sigint(void);
void				set_exec_sigint(void);
void				set_main_sigquit(void);
void				set_exec_sigquit(void);

// env
void				*ft_env(t_env_op op, char *str);
t_env				*search_val(t_env *head, char *name,
						int offset, t_val_type type);
char				*rm_quotes(char *val);
t_env				*ft_add_env(t_env **head, char *str, int offset);
t_env				*ft_update_env(t_env **head, char *str, int offset);
t_env				*env_del_all(t_env **head);
t_env				*env_del(t_env **head, char *name);
void				*get_env_all(t_env **head, t_env_op op);
void				*get_env(t_env *head, char *name, t_env_op op);

// utils
int					sh_stat(t_st_op op, int val);
char				*ft_strndup(char *s, int len);
char				*ft_union(char **split, char delim);
char				*ft_cwd(t_pwd_op op, char *path);
int					ft_isspace(char c);
int					is_valid_env(char *name);
int					event(void);
int					ft_split_len(char **split);
void				*ft_malloc(size_t size);
void				interpret(t_shell *shell);
void				exit_shell(int print);
void				finish_loop(t_shell *shell);
char				**get_longer_split(char **split, char *new, int len);
long long int		ft_atol(char *num, long long int minus, int digit);
char				*ft_strjoin_delim(char *s1, char delim, char *s2);

// free
void				free_2d_array(char **array);
void				free_tokens(t_token *token);
void				free_redirs(t_redir **redirs);
void				free_ast(t_node *ast);
void				free_shell(t_shell **shell);
void				free_env(t_env *env);

// debug
void				print_tokens(t_token *token);
void				debug_tokenizer(t_token *tokens);
void				print_indent(int depth, bool is_last);
void				print_redir(t_redir *redir, int depth);
void				print_ast(t_node *node, int depth);
void				debug_parser(t_node *ast);
void				debug_expand(t_node *ast);
void				debug_exec_list(t_node *node);
void				print_exp_token_list(t_exp_tkn *head);
void				print_2d_array(char **array);

#endif