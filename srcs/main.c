/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 03:56:43 by ogenc             #+#    #+#             */
/*   Updated: 2023/10/25 17:05:18 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	find_env_dir(char **env_p, char *find) // bu fonksiyon env nin içerisinde satır satır arama işlemi
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

void	free_commands(char **commands) // bu komut char ** lari kolayca freeleyip leaks bırakmamak için yapıldı.
{
	int	x;

	x = -1;
	while (commands[++x])
		free(commands[x]);
	free(commands);
}

// TODO 2: x
// builtin fonksiyonları yazılacak, ayırılacak
// cd komutunun tek "cd" argümanında home dizini
// todo:
// export unset pipes redirections parser

char	*find_access(t_exec *data, char *input) // bu fonksiyon gelen envp PATH= den itibaren gelen inputun örnek "ls" inputunun path'te hangi bölgede oldugunu döndürür.
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

	i = 1;
	if (commands[1] && !ft_strcmp(commands[1], "-n"))
	{
		i++;
		while (commands[i])
		{
			printf("%s", commands[i]);
			if (commands[i + 1])
				printf(" ");
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
}

void	ft_pwd(char **commands, t_exec *data)
{
	char	*t_pwd;

	(void)data;
	t_pwd = malloc(sizeof(char) * 1024);
	if (!getcwd(t_pwd, 1024))
		perror("error");
	if (commands[1])
		printf("pwd: too many arguments\n");
	else
		printf("%s\n", t_pwd);
	free(t_pwd);
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
	char	*new_pwd;
	char	*old_pwd;

	t_pwd = malloc(sizeof(char) * 1024);
	if (chdir(token) == -1)
		return (-1);
	else
	{
		if (!getcwd(t_pwd, 1024))
			perror("error");
		else
		{
			new_pwd = ft_strjoin("PWD=", t_pwd);
			old_pwd = ft_strjoin("OLD", data->env_p[find_env_dir(data->env_p, "PWD")]);
			data->env_p[find_env_dir(data->env_p, "PWD")] = new_pwd;
			data->env_p[find_env_dir(data->env_p, "OLDPWD")] = old_pwd;
		}
	}
	free(t_pwd);
	return (0);
}

void	ft_set_export(t_exec *data, char *export)
{
	int i;

	i = 0;
	while (data->env_p[i])
		i++;
	data->env_p[i] = malloc(sizeof(char) * ft_strlen(export) + 1);
	ft_strlcpy(data->env_p[i], export, ft_strlen(export) + 1);
	data->env_p[i + 1] = NULL;
}

char	*ft_f_command(char *command)
{
	char *n_str;
	int i;

	i = 0;
	while ((command[i] >= 'A' && command[i] <= 'Z') || (command[i] >= 'a' && command[i] <= 'z')) // kücük A kücük Z 
		i++;
	if (command[i] == '=')
	{
		n_str = malloc(sizeof(char) * i + 1);
		ft_strlcpy(n_str, command, i + 1);
	}
	else
		return(NULL);
	return (n_str);
	
}

int	ft_export(t_exec *data, char **commands)
{
	int x;
	int j;
	char	*command_w_eq;

	x = 1;
	j = 0;
	while (commands[x])
	{
		command_w_eq = ft_f_command(commands[x]);
		if (commands[x][0] >= '0' && commands[x][0] <= '9')
		{
			printf("export: not an identifier: %s\n", commands[x]);
			return (-1);
		}
		j = 0;
		if (find_env_dir(data->env_p, command_w_eq) != -1) // cheak memory leaks
			ft_strlcpy(data->env_p[find_env_dir(data->env_p, command_w_eq)], commands[x], ft_strlen(commands[x]) + 1);
		else
		{
			while (commands[x][j])
			{
				if ((commands[x][j] >= 'a' && commands[x][j] <= 'z') || (commands[x][j] >= 'A' && commands[x][j] <= 'Z'))
					j++;
				else if (commands[x][j] == '=')
				{
					ft_set_export(data, commands[x]);
					break;
				}
				else
					return (-1);
			}
		}
		free(command_w_eq);
		x++;
	}
	return (-1);
}

void	ft_p_env(t_exec *data)
{
	int i;

	i = 0;
	while(data->env_p[i])
	{
		printf("%s\n",data->env_p[i]);
		i++;
	}
}

void	ft_exec_w_pipes(t_exec *data, char **commands)
{
	int total_pipe;
	int total_exec;
	pid_t pid;
	int in = 0;
	total_pipe = g_data.counter->pipe;
	total_exec = total_pipe + 1;
	
	while (total_pipe >= 0)
	{
		pipe(g_data.fd);
		pid = fork();
		if (!pid)
		{
			data->path = ft_join_m(data, commands);
			dup2(in, 0);
			if (total_pipe - 1 != -1)
				dup2(g_data.fd[1], 1);
			close(g_data.fd[0]);
			close(g_data.fd[1]); 
			// if is builtin
			execve(data->path, commands, data->env_p);
			perror("Invalid command");
			exit(1);
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
		if (g_data.arg->next)
		{
			commands = g_data.arg->next->content;
			g_data.arg = g_data.arg->next;
		}
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
	int i = find_env_dir(data->env_p, commands[1]);
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
	int i = 0;
	while(envp[i])
		i++;
	data->env_p = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		data->env_p[i] = ft_strdup(envp[i]);
		i++;
	}
	data->env_p[i] = NULL;
}

int	main (int argc, char **argv, char **env)
{
	char	**commands;
	int		pid;
	t_exec	*data;

	(void)argv;
	(void)argc;
	data = malloc(sizeof(data));
	envp_copy(env);
	set_envp(data, env);
	while (1)
	{
		g_data.line = readline("\033[34mminishell \033[0;35m$ \033[0m");
		if (ft_strncmp(g_data.line, "", ft_strlen(g_data.line)) != 0) // add history
			add_history(g_data.line);
		ft_parse();
		if (!(ft_strncmp(g_data.line, "\0", 1) == 0) && g_data.error_flag == 0)
		{
			commands = g_data.arg->content;
			if (ft_strcmp(commands[0], "exit") == 0) // exit
			{
				printf("\033[31mExiting minishell...\033[0m\n");
				free_commands(commands);
				free(g_data.line);
				free(data);
				exit(0);
			}
			else if (g_data.counter->pipe > 0)
				ft_exec_w_pipes(data, commands);
			else if (ft_strcmp(commands[0], "cd") == 0) // cd komutuna özel 
			{
				if (commands[1])
					if (ft_change_dir(data, commands[1]) == -1)
						printf("\033[31mcd: no such file or directory: %s\033[0m\n", commands[1]);
			}
			else if (!(ft_strcmp(commands[0], "echo")))
				ft_echo(commands);
			else if (!(ft_strcmp(commands[0], "pwd"))) // pwd
				ft_pwd(commands, data);
			else if (!(ft_strcmp(commands[0], "export")))
				ft_export(data, commands);
			else if (!(ft_strcmp(commands[0], "unset")))
				ft_unset(data, commands);
			else if (!(ft_strcmp(commands[0], "env")))
				ft_p_env(data);
			else
			{
				data->path = ft_join_m(data, commands);
				pid = fork();
				if (pid == 0)
				{
					if (execve(data->path, commands, data->env_p) == -1)
						perror("Invalid command");
					exit(1);
				}
				else
					waitpid(pid, NULL, 0);
				free(data->path);
			}
			free_commands(commands);
		}
		free(g_data.line);
		free(g_data.counter);
		freelizer(&g_data.list);
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
	g_data.quot_type = 1000;
}