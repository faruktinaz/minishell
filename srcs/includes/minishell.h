/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/30 14:31:07 by ogenc            ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdarg.h>
# include <string.h>
# include <stdbool.h>
#include <sys/ioctl.h>
enum	character {
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
	int				count;
	struct s_newlst	*next;
}					t_newlst;

typedef struct s_exec
{
	char	**env_p;
	char 	*path;
}	t_exec;

typedef struct s_data
{
	t_counter	*counter;
	t_arg		*list;
	t_newlst	*arg;
	char		*line;
	char		**envp;
	char		*path;
	char		**redirection;
	int			error_flag;
	int			error_code;
	int			quot;
	int			fd[2];
    int         in_fd;
    int         out_fd;
	int			in_rdr;
    int         fdin;
    int         fdout;
	int			tmp;
	int			quot_type;
    int         default_in;
    int         default_out;
    int         exec_check;
}				t_data;

t_data	g_data;


void	ft_parse(void);
void	freelizer(t_arg **line, t_newlst **line2);
int		is_check(char c);
void	struct_initilaize(char **envp, int rule);
bool	env_check(char const *str, char c, int rule);
char	*env_find(char *path);
void	check_quot_list(t_arg *temp);
char	*env_add_dollars(char *str, char *path);
int		env_control(char *str, int i);
void	ft_error(char *str);
void	change_list(t_arg *temp);
void	find_env_name(t_arg *temp);
void	parse_error(int error_code);
void	*error_check(t_arg *temp);
t_arg	*ms_lstnew(int type, void *content);
void	ms_lstadd_back(t_arg **lst, t_arg *new);
t_arg	*ms_lstlast(t_arg *lst);
char	*ms_strjoin(char *s1, char *s2);
void	make_sense(t_arg **list);
void	ft_exec_rdr(t_newlst **list);
char	*ft_join_m(t_exec *data, char **commands);
void	free_commands(char **commands);
void	double_input_rdr(t_newlst *tmp, int i);
int	    ft_strcmp(char *s1, char *s2);
void    change_output_or_input(void);

#endif
