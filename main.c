/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 03:56:43 by ogenc             #+#    #+#             */
/*   Updated: 2023/09/23 18:31:23 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include"./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_data
{
	char	**env_p;
	char 	*path;
}	t_data;

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

char	*find_access(t_data *data, char *input) // bu fonksiyon gelen envp PATH= den itibaren gelen inputun örnek "ls" inputunun path'te hangi bölgede oldugunu döndürür.
{
	char	**env_path;
	char	*str;
	char	*str_a;
	int		x;
	int		i;

	x = find_env_dir(data->env_p, "PATH");
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

void	ft_pwd(char **commands, t_data *data)
{
	if (commands[1])
		printf("pwd: too many arguments\n");
	else
		printf("%s\n", data->env_p[find_env_dir(data->env_p, "PWD")] + 4);
}

char	*ft_join_m(t_data *data, char **commands)
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

int	ft_change_dir(t_data *data, char *token)
{
	char	*test;
	char	*new_pwd;
	char	*old_pwd;

	test = malloc(sizeof(char) * 1024);
	if (chdir(token) == -1)
		return (-1);
	else
	{
		if (!getcwd(test, 1024))
			perror("error");
		else
		{
			new_pwd = ft_strjoin("PWD=", test);
			old_pwd = ft_strjoin("OLD", data->env_p[find_env_dir(data->env_p, "PWD")]);
			data->env_p[find_env_dir(data->env_p, "PWD")] = new_pwd;
			data->env_p[find_env_dir(data->env_p, "OLDPWD")] = old_pwd;
		}
	}
	free(test);
	return (1);
}

void	ft_set_export(t_data *data, char *export)
{
	int i;

	i = 0;
	while (data->env_p[i])
		i++;
	data->env_p[i] = malloc(ft_strlen(export));
	ft_strlcpy(data->env_p[i], export, ft_strlen(export) + 1);
	data->env_p[i + 1] = NULL;
}

char	*ft_f_command(char *command)
{
	char *n_str;
	int i;

	i = 0;
	while (command[i] >= 'A' && command[i] <= 'Z') // kücük A kücük Z 
		i++;
	if (command[i] == '=')
	{
		n_str = malloc(sizeof(char) * i);
		ft_strlcpy(n_str, command, i);
	}
	else
		return(NULL);
	return (n_str);
	
}

int	ft_export(t_data *data, char **commands)
{
	int x;
	int j;

	x = 1;
	j = 0;
	while (commands[x])
	{
		if (commands[x][0] >= '0' && commands[x][0] <= '9')
		{
			printf("export: not an identifier: %s\n", commands[x]);
			return (-1);
		}
		j = 0;
		if (find_env_dir(data->env_p, ft_f_command(commands[x])) != -1) // cheak memory leaks
			ft_strlcpy(data->env_p[find_env_dir(data->env_p, ft_f_command(commands[x]))], commands[x], ft_strlen(commands[x]) + 1);
		else
		{
			while (commands[x][j])
			{
				if (commands[x][j] >= 'a' && commands[x][j] <= 'z')
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
		x++;
	}
	return (-1);
}

void	ft_p_env(t_data *data)
{
	int i;

	i = 0;
	while(data->env_p[i])
	{
		printf("%s\n",data->env_p[i]);
		i++;
	}
}

int	search_pipes(t_data *data, char **commands)
{
	int i = 0;
	int counter = 0;
	while (commands[i])
	{
		if (!ft_strcmp(commands[i], "|"))
			counter++; 
		i++;
	}
	return (counter);
}

void	ft_exec_w_pipes(t_data *data, char **commands) //  when parser added to minishell then this function will changed. 0. 1. | 0. 1. or 0. 1. | 2. 3. which one comes from parser
{
	int total_pipe;
	int total_exec;
	int fd[2];
	int pid;
	int i = 0;
	

	data->path = ft_join_m(data, commands);
	total_pipe = search_pipes(data, commands);
	total_exec = total_pipe + 1;
	pipe(fd);
	while (total_pipe >= 0)
	{
		pid = fork();
		if (pid == 0)
		{
			if (ft_strcmp(commands[i], "|") == 0)
				i++;
			if (!(ft_strcmp(commands[i], "env")))
				ft_p_env(data);
			else if (!(ft_strcmp(commands[i], "echo")))
				ft_echo(commands);
			else if (!(ft_strcmp(commands[i], "pwd")))
				ft_pwd(commands, data);
		}
		else
			waitpid(pid, NULL, 0);
		i++;
		total_pipe--;
	}
}

void	ft_unset(t_data *data, char **commands)
{
	int i = 0;
	if (find_env_dir(data->env_p, ft_f_command(commands[1])) != -1)
	{
		i = find_env_dir(data->env_p, ft_f_command(commands[1]));
		while (data->env_p[i + 1])
		{
			data->env_p[i] = data->env_p[i + 1];
			i++;
		}
		data->env_p[i] = NULL;
	}
}

int	main (int argc, char **argv, char **env)
{
	char	*input;
	char	**commands;
	int		pid;
	t_data	*data;

	(void)argv;
	(void)argc;
	data = malloc(sizeof(data));
	data->env_p = env;
	while (1)
	{
		input = readline("\033[34mminishell \033[0;35m$ \033[0m");
		if (!(ft_strncmp(input, "\0", 1) == 0))
		{
			commands = ft_split(input, ' ');
			if (ft_strncmp(input, "", ft_strlen(input)) != 0) // add history
				add_history(input);
			if (ft_strcmp(commands[0], "exit") == 0) // exit
			{
				printf("\033[31mExiting minishell...\033[0m\n");
				free_commands(commands);
				free(input);
				free(data);
				exit(0);
			}
			else if (search_pipes(data, commands) > 0)
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
		free(input);
	}
}
