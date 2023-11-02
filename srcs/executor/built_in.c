/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:05:39 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 08:44:12 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(char **commands)
{
	int	i;
	int	j;
	int	b_check;
	int	n_added;

	i = 1;
	b_check = 2;
	n_added = 0;
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
	data->res = 1;
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
	data->res = 1;
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
	data->res = 1;
}

void	ft_cd(t_exec *data, char **content)
{
	char	*tmp;

	tmp = NULL;
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
			tmp = ft_strjoin(g_data.envp[find_env_dir(g_data.envp, "HOME")] \
					+ 5, "/");
			if (ft_change_dir(data, tmp) == -1)
				printf("HOME not set %s\n", content[0]);
			free(tmp);
		}
		else
			printf("HOME not set %s\n", content[0]);
	}
	data->res = 1;
}
