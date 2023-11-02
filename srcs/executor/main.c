/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 03:56:43 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 07:40:29 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	find_env_dir(char **env_p, char *find)
{
	int	j;

	j = 0;
	if (find)
	{
		while (env_p[j])
		{
			if (ft_strncmp(env_p[j], find, ft_strlen(find)) == 0)
				return (j);
			else
				j++;
		}
	}
	return (-1);
}

void	free_commands(char **commands)
{
	int	x;

	x = -1;
	while (commands[++x])
		free(commands[x]);
	free(commands);
}

char	*find_access(t_exec *data, char *input)
{
	char	**env_path;
	char	*str;
	char	*str_a;
	int		x;
	int		i;

	x = find_env_dir(data->env_p, "PATH");
	if (x != -1)
	{
		env_path = ft_split(data->env_p[x] + 5, ':');
		i = 0;
		while (env_path[i])
		{
			str_a = ft_strjoin(env_path[i], input);
			if (access(str_a, F_OK) == 0)
			{
				str = env_path[i];
				str = ft_strjoin(str, "/");
				free_commands(env_path);
				free(str_a);
				return (str);
			}
			free(str_a);
			i++;
		}
		free_commands(env_path);
	}
	return ("error");
}

void	ft_echo(char **commands)
{
	int	i;
	int	j;
	int	b_check = 2;
	int	n_added = 0;

	i = 1;
	j = 0;
	if (commands[1] && !ft_strncmp(commands[1], "-n", 2))
	{
		j = 2;
		while (commands[1][j])
		{
			if (commands[1][j] != 'n')
				b_check = 0;
			j++;
		}
		b_check = 1;
	}
	if (commands[1] && b_check == 1)
	{
		i++;
		while (commands[i])
		{
			if (!ft_strncmp(commands[i], "-n", 2) && !n_added)
			{
				j = 2;
				while (commands[i][j])
				{
					if (commands[i][j] != 'n')
					{
						printf("%s", commands[i]);
						if (commands[i + 1])
							printf(" ");
						n_added = 1;
						break ;
					}
					else
						break ;
				}
			}
			else
			{
				printf("%s", commands[i]);
				if (commands[i + 1])
					printf(" ");
			}
			i++;
		}
	}
	else
	{
		while (commands[i])
		{
			printf("%s", commands[i]);
			if (commands[i + 1])
				printf(" ");
			i++;
		}
		printf("\n");
	}
	g_data.error_code = 0;
}

void	ft_pwd(t_exec *data)
{
	char	*t_pwd;

	(void)data;
	t_pwd = malloc(sizeof(char) * 1024);
	if (!getcwd(t_pwd, 1024))
		perror("error");
	else
		printf("%s\n", t_pwd);
	free(t_pwd);
	g_data.error_code = 0;
}

char	*ft_join_m(t_exec *data, char **commands)
{
	char	*str;
	char	*str1;
	char	*str2;

	str = ft_strjoin("/", commands[0]);
	str2 = find_access(data, str);
	str1 = ft_strjoin(str2, commands[0]);
	free(str);
	if (ft_strcmp(str2, "error"))
		free(str2);
	return (str1);
}

int	ft_change_dir(t_exec *data, char *token)
{
	char	*t_pwd;
	int		i_pwd;
	char	*new_pwd;
	char	*old_pwd;

	t_pwd = malloc(sizeof(char) * 1024);
	if (chdir(token) == -1)
	{
		free(t_pwd);
		return (-1);
	}
	else
	{
		if (!getcwd(t_pwd, 1024))
			perror("error");
		else
		{
			i_pwd = find_env_dir(data->env_p, "PWD");
			new_pwd = ft_strjoin("PWD=", t_pwd);
			if (i_pwd == -1)
				old_pwd = ft_strjoin("OLDPWD=", " ");
			else
				old_pwd = ft_strjoin("OLDPWD=", data->env_p[i_pwd] + 4);
			if (i_pwd != -1)
				ft_strlcpy(data->env_p[i_pwd], new_pwd, ft_strlen(new_pwd) + 1);
			ft_strlcpy(data->env_p[find_env_dir(data->env_p, "OLDPWD")], \
							old_pwd, ft_strlen(old_pwd) + 1);
			free(new_pwd);
			free(old_pwd);
		}
	}
	free(t_pwd);
	return (0);
}

int		check_is_dir(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == '/')
			res = 1;
		i++;
	}
	return (res);
}

void	ft_set_export(t_exec *data, char *export)
{
	int	i;

	i = 0;
	while (data->env_p[i])
		i++;
	data->env_p[i] = malloc(sizeof(char) * ft_strlen(export) + 1);
	ft_strlcpy(data->env_p[i], export, ft_strlen(export) + 1);
}

char	*ft_f_command(char *command)
{
	char	*n_str;
	int		i;

	i = 0;
	while ((command[i] >= 'A' && command[i] <= 'Z') \
			|| (command[i] >= 'a' && command[i] <= 'z'))
		i++;
	if (command[i] == '=' && command[0] != '=')
	{
		n_str = malloc(sizeof(char) * i + 1);
		ft_strlcpy(n_str, command, i + 1);
	}
	else
		return (NULL);
	return (n_str);
}

void	ft_p_env_ex(t_exec *data)
{
	int	i;

	i = 0;
	while (data->env_p[i])
	{
		printf("declare -x %s\n", data->env_p[i]);
		i++;
	}
}

int	ft_export(t_exec *data, char **commands)
{
	int		x;
	int		j;
	char	*command_w_eq;

	x = 1;
	j = 0;
	if (!commands[1])
		ft_p_env_ex(data);
	while (commands[x])
	{
		command_w_eq = ft_f_command(commands[x]);
		if (commands[x][0] >= '0' && commands[x][0] <= '9')
		{
			printf("export: not an identifier: %s\n", commands[x]);
			g_data.error_code = 256;
			return (-1);
		}
		j = 0;
		if (command_w_eq && find_env_dir(data->env_p, command_w_eq) != -1)
			ft_strlcpy(data->env_p[find_env_dir(data->env_p, command_w_eq)], \
					commands[x], ft_strlen(commands[x]) + 1);
		else
		{
			while (commands[x][j])
			{
				if ((commands[x][j] >= 'a' && commands[x][j] <= 'z') \
						|| (commands[x][j] >= 'A' && commands[x][j] <= 'Z'))
					j++;
				else if (commands[x][j] == '=' && commands[x][0] != '=')
				{
					ft_set_export(data, commands[x]);
					break ;
				}
				else
				{
					printf("not a valid identifier %s\n", commands[1]);
					return (-1);
				}
			}
		}
		free(command_w_eq);
		x++;
	}
	return (0);
}

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		if (g_data.in_rdr == 2)
		{
			g_data.in_rdr = 5;
			ioctl(0, TIOCSTI, "\n");
		}
		else
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (signum == SIGQUIT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_p_env(t_exec *data)
{
	int	i;

	i = 0;
	while (data->env_p[i])
	{
		printf("%s\n", data->env_p[i]);
		i++;
	}
	g_data.error_code = 0;
}

void	ft_exec_w_pipes(t_exec *data, char **commands)
{
	t_newlst	*tmp;
	int			total_pipe;
	int			total_exec;
	int			in;
	pid_t		pid;

	tmp = g_data.arg;
	total_pipe = g_data.counter->pipe;
	total_exec = total_pipe + 1;
	in = 0;
	(void)commands;
	while (total_pipe >= 0)
	{
		pipe(g_data.fd);
		pid = fork();
		if (!pid)
		{
			if (check_is_dir(tmp->content[0]) == 1)
				g_data.err_ty = 1;
			if (access(tmp->content[0], F_OK) != 0)
				data->path = ft_join_m(data, tmp->content);
			else
				data->path = ft_strdup(tmp->content[0]);
			dup2(in, 0);
			if (total_pipe - 1 != -1)
				dup2(g_data.fd[1], 1);
			close(g_data.fd[0]);
			close(g_data.fd[1]);
			if (tmp->list_type != WORD)
			{
				ft_exec_rdr(&tmp);
				change_output_or_input();
			}
			if (tmp->content[0] && is_built_in(data, tmp->content) != 1)
			{
				if (execve(data->path, tmp->content, data->env_p) == -1)
				{
					errno = 127;
					if (g_data.err_ty == 0)
						printf("command not found: %s\n", tmp->content[0]);
					else if (g_data.err_ty == 1)
						printf("No such file or directory: %s\n", \
							tmp->content[0]);
				}
			}
			exit(errno);
		}
		else
		{
			if (in != 0)
				close(in);
			if (total_pipe != 0)
				in = dup(g_data.fd[0]);
			close(g_data.fd[1]);
			close(g_data.fd[0]);
		}
		total_pipe--;
		if (tmp->next)
			tmp = tmp->next;
	}
	total_pipe = total_exec - 1;
	while (total_pipe >= 0)
	{
		waitpid(-1, NULL, 0);
		total_pipe--;
	}
	return ;
}

void	ft_unset(t_exec *data, char **commands)
{
	int	i;

	i = find_env_dir(data->env_p, commands[1]);
	if (i != -1)
	{
		free(data->env_p[i]);
		while (data->env_p[i + 1] != NULL)
		{
			data->env_p[i] = data->env_p[i + 1];
			i++;
		}
		data->env_p[i] = NULL;
	}
}

void	envp_copy(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
		i++;
	g_data.envp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		g_data.envp[i] = ft_strdup(envp[i]);
		i++;
	}
	g_data.envp[i] = NULL;
}

void	set_envp(t_exec *data, char **envp)
{
	int	i;

	i = 0;
	while(envp[i])
		i++;
	data->env_p = calloc(1024, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		data->env_p[i] = ft_strdup(envp[i]);
		i++;
	}
	data->env_p[i] = NULL;
}

void    change_output_or_input(void)
{
	if (g_data.fdout == 1)
	{
		dup2(g_data.out_fd, 1);
		close(g_data.out_fd);
	}
}

static void	delete_hat(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p) != 0)
		perror("Minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p) != 0)
		perror("Minishell: tcsetattr");
}

int	is_built_in(t_exec *data, char **content)
{
	int		result;
	char	*tmp;
	int		b;

	result = 0;
	b = 0;
	tmp = NULL;
	if (ft_strcmp(content[0], "exit") == 0)
	{
		if (!(g_data.counter->pipe > 0))
		{
			printf("\033[31mExiting minishell...\033[0m\n");
			if (content[1])
			{
				b = ft_atoi(content[1]);
				if (b < 0)
					b = -b;
			}
			if (b != 0)
				exit(b);
			exit(0);
		}
		result = 1;
	}
	else if (ft_strcmp(content[0], "cd") == 0)
	{
		if (content[1])
		{
			if (!ft_strcmp(content[1], "~"))
				ft_change_dir(data, getenv("HOME"));
			else if (ft_change_dir(data, content[1]) == -1)
				printf("\033[31mcd: no such file or directory: %s\033[0m\n", \
						content[1]);
		}
		else if (!content[1])
		{
			if (find_env_dir(g_data.envp, "HOME") != -1)
			{
				tmp = ft_strjoin(g_data.envp[find_env_dir(g_data.envp, "HOME")] + 5, "/");
				if (ft_change_dir(data, tmp) == -1)
					printf("HOME not set %s\n", content[0]);
				free(tmp);
			}
			else
				printf("HOME not set %s\n", content[0]);
		}
		result = 1;
	}
	else if (!(ft_strcmp(content[0], "echo")))
	{
		ft_echo(content);
		result = 1;
	}
	else if (!(ft_strcmp(content[0], "pwd")))
	{
		ft_pwd(data);
		result = 1;
	}
	else if (!(ft_strcmp(content[0], "export")))
	{
		ft_export(data, content);
		result = 1;
	}
	else if (!(ft_strcmp(content[0], "unset")))
	{
		result = 1;
		ft_unset(data, content);
	}
	else if (!(ft_strcmp(content[0], "env")))
	{
		ft_p_env(data);
		result = 1;
	}
	return (result);
}

int	main (int argc, char **argv, char **env)
{
	int		pid;
	t_exec	*data;

	(void)argv;
	(void)argc;
	struct_initilaize(NULL, 1);
	data = malloc(sizeof(t_exec));
	data->t_exp = malloc(sizeof(t_list));
	set_envp(data, env);
	g_data.envp = data->env_p;
	g_data.default_in = dup(0);
	g_data.default_out = dup(1);
	delete_hat();
	signal(SIGINT, &handle_signals);
	signal(SIGQUIT, &handle_signals);
	signal(EOF, &handle_signals);
	while (1)
	{
		g_data.line = readline("\033[34mminishell \033[0;35m$ \033[0m");
		if (!g_data.line)
		{
			write(1, "exit", 4);
			exit(0);
		}
		if (ft_strncmp(g_data.line, "", ft_strlen(g_data.line)) != 0)
			add_history(g_data.line);
		ft_parse();
		if (!(ft_strncmp(g_data.line, "\0", 1) == 0) && g_data.error_flag == 0)
		{
			if (g_data.arg->list_type != WORD && g_data.counter->pipe < 1)
			{
				ft_exec_rdr(&g_data.arg);
				change_output_or_input();
			}
			if (g_data.in_rdr == 1)
			{
				g_data.in_rdr = 0;
				continue ;
			}
			if (g_data.counter->pipe > 0)
				ft_exec_w_pipes(data, g_data.arg->content);
			else if (g_data.arg->content[0] && is_built_in(data, g_data.arg->content) != 1)
			{
				if (check_is_dir(g_data.arg->content[0]) == 1)
					g_data.err_ty = 1;
				if (access(g_data.arg->content[0], F_OK))
					data->path = ft_join_m(data, g_data.arg->content);
				else
					data->path = ft_strdup(g_data.arg->content[0]);
				pid = fork();
				if (pid == 0)
				{
					if (execve(data->path, g_data.arg->content, data->env_p) == -1)
					{
						errno = 127;
						if (g_data.err_ty == 0)
							printf("command not found: %s\n", g_data.arg->content[0]);
						else if (g_data.err_ty == 1)
							printf("No such file or directory: %s\n", g_data.arg->content[0]);
					}
					exit(errno);
				}
				else
					waitpid(pid, &g_data.error_code, 0);
				free(data->path);
			}
		}
		free(g_data.line);
		free(g_data.counter);
		freelizer(&g_data.list, &g_data.arg);
		struct_initilaize(NULL, 0);
	}
}

void	struct_initilaize(char **envp, int rule)
{
	(void)envp;
	if (rule == 1)
		g_data.error_code = 0;
	g_data.error_flag = 0;
	g_data.quot = 0;
	g_data.in_rdr = 0;
	g_data.quot_type = 1000;
	g_data.fdin = 0;
	g_data.fdout = 0;
	g_data.err_ty = 0;
	g_data.exec_check = 0;
	dup2(g_data.default_in, 0);
	dup2(g_data.default_out, 1);
}
