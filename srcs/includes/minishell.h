/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 03:41:51 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libary/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdarg.h>
# include <string.h>
# include <stdbool.h>
# include <termios.h>

enum	e_character {
	WORD,
	INPUT_RDR,
	OUTPUT_RDR,
	DOUBLE_OUTPUT_RDR,
	DOUBLE_INPUT_RDR,
	PIPE,
};

typedef struct s_counter
{
	int	rdr;
	int	heredoc;
	int	pipe;
	int	word;
}		t_counter;

typedef struct s_arg
{
	char			*content;
	int				type;
	struct s_arg	*next;
}					t_arg;

typedef struct s_newlst
{
	char			**content;
	int				*type;
	int				list_type;
	struct s_newlst	*next;
}					t_newlst;

typedef struct s_exec
{
	char	**env_p;
	char	*path;
	char	*new_pwd;
	char	*old_pwd;
	int		res;
	t_list	*t_exp;

}	t_exec;

typedef struct s_data
{
	t_counter	*counter;
	t_arg		*list;
	t_newlst	*arg;
	t_arg		*exp_p;
	char		*line;
	char		**envp;
	char		*path;
	char		**redirection;
	int			error_flag;
	int			error_code;
	int			quot;
	int			fd[2];
	int			in_fd;
	int			out_fd;
	int			x;
	int			in_rdr;
	int			b_check;
	int			n_added;
	int			fdin;
	int			fdout;
	int			tmp;
	int			quot_type;
	int			err_ty;
	int			default_in;
	int			default_out;
	int			exec_check;
	int			dollars_error;
	int			e_check;
	char		*parse_str;
	char		*input_name;
}				t_data;

t_data	g_data;

// parser part

void	ft_parse(void);
void	freelizer(t_arg **line, t_newlst **line2);
int		is_check(char c);
void	struct_initilaize(char **envp, int rule);
bool	env_check(char const *str, char c, int rule);
char	*env_find(char *path);
void	check_quot_list(t_newlst **temp);
int		is_built_in(t_exec *data, char **content);
char	*env_add_dollars(char *str, char *path, int check);
char	*env_add_dollars3(char *str, char *tmp_path, char *path, int i);
int		env_control(char *str, int i);
void	ft_error(char *str);
void	change_list(t_arg *temp);
int		find_env_dir(char **env_p, char *find);
void	find_env_name(t_arg *temp);
void	parse_error(int error_code, char *str);
int		is_oparators(char *str, int i, int oparator, int rule);
int		error_check(t_arg *temp);
t_arg	*ms_lstnew(int type, void *content);
void	ms_lstadd_back(t_arg **lst, t_arg *new);
t_arg	*ms_lstlast(t_arg *lst);
char	*ms_strjoin(char *s1, char *s2);
int		ms_lstsize(t_arg *lst);
void	make_sense(t_arg **list);
void	ft_exec_rdr(t_newlst **list);
char	*ft_join_m(t_exec *data, char **commands);
void	free_commands(char **commands);
void	double_input_rdr(t_newlst *tmp, int i);
int		ft_strcmp(char *s1, char *s2);
void	change_output_or_input(void);
void	splitting_to_add_list(t_arg *temp, char *str);
int		ms_lstsize(t_arg *lst);
char	**change_newlst(t_newlst *tmp, int count, int check);
void	change_newlst_rdr_not_start(t_newlst *tmp \
	, char ***new_str, int lenght, int count);
void	change_newlst_rdr_start(t_newlst *tmp \
	, char ***new_str, int count);
int		last_rdr_check(t_newlst *tmp, int i, int check);

// exec part

int		is_built_in(t_exec *data, char **content);
char	*ft_join_m(t_exec *data, char **commands);
int		find_env_dir(char **env_p, char *find);
void	free_commands(char **commands);
void	ft_exec_w_pipes(t_exec *data);
void	ft_cd(t_exec *data,	char **content);
int		is_built_in(t_exec *data, char **content);
void	ft_pwd(t_exec *data);
void	ft_echo(char **commands);
void	ft_unset(t_exec *data, char **commands);
void	ft_unset2(char **commands);
void	ft_p_env_ex(t_exec *data);
void	ft_check_f(t_exec *data);
int		ft_echo_2(char **commands, int i);
void	ft_exec_rdr(t_newlst **list);
void	ft_p_env(t_exec *data);
int		ft_export(t_exec *data, char **commands);
int		ft_change_dir(t_exec *data, char *token);
void	ft_exit(t_exec *data, char **content);
void	ft_set_export(t_exec *data, char *export, int j);
int		ft_change_dir(t_exec *data, char *token);
int		find_env_dir(char **env_p, char *find);
void	ft_exc_chdir(t_exec *data, int i_pwd);
int		check_is_dir(char *str);
void	ft_p_env_ex(t_exec *data);
void	change_output_or_input(void);
void	handle_signals(int signum);
void	ft_execute(t_exec *data, t_newlst *tmp);
void	delete_hat(void);
char	*find_access(t_exec *data, char *input);
int		ft_sig_rdr_h(void);
void	set_envp(t_exec *data, char **envp);
char	*ft_f_command(char *command);
void	ft_readline(t_exec *data);
void	init_signals(void);
int		find_env_dir2(t_arg *expath, char *find);
void	change_exp_content(t_arg *expath, int type, char *commands);
void	ft_unset2(char **content);

#endif