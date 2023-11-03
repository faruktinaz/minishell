/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:25:46 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 03:39:41 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_exec *data, char **content)
{
	int	b;

	b = 0;
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
	data->res = 1;
}

int	ft_ex_c(char **commands, int x, char *command_w_eq)
{
	if (commands[x][0] >= '0' && commands[x][0] <= '9')
	{
		printf("export: not an identifier: %s\n", commands[x]);
		g_data.error_code = 256;
		free(command_w_eq);
		return (-1);
	}
	return (0);
}

int	ft_export_2(t_exec *data, char **commands, int x, char *command)
{
	int	j;
	int	size;

	j = 0;
	while (commands[x][j])
	{
		size = ft_strlen(commands[x]);
		if ((size > 1 && commands[x][j] >= 'a' && commands[x][j] <= 'z') \
				|| (commands[x][j] >= 'A' && commands[x][j] <= 'Z'))
			j++;
		else if (commands[x][0] != '=')
		{
			if (g_data.e_check == 0)
				ft_set_export(data, commands[x], j);
			break ;
		}
		else
		{
			printf("not a valid identifier %s\n", commands[1]);
			free(command);
			return (-1);
		}
	}
	return (0);
}

int	ft_u_export(char **commands, char *command_w_e, int x)
{
	if (ft_ex_c(commands, x, command_w_e) == -1)
		return (-1);
	if (command_w_e && find_env_dir2(g_data.exp_p, command_w_e) != -1)
	{
		if (ft_strlen(commands[x]) == 1)
			g_data.e_check = 1;
		change_exp_content(g_data.exp_p, \
			find_env_dir2(g_data.exp_p, command_w_e), commands[x]);
	}
	return (0);
}

int	ft_export(t_exec *data, char **commands)
{
	int		x;
	char	*command_w_e;

	x = 0;
	g_data.e_check = 0;
	if (!commands[1])
		ft_p_env_ex(data);
	while (commands[++x])
	{
		command_w_e = ft_f_command(commands[x]);
		if (ft_u_export(commands, command_w_e, x) == -1)
			return (-1);
		if (command_w_e && find_env_dir(data->env_p, command_w_e) != -1)
		{
			if (ft_strlen(commands[x]) > 1)
				ft_strlcpy(data->env_p[find_env_dir(data->env_p, command_w_e)], \
						commands[x], ft_strlen(commands[x]) + 1);
		}
		else
			if (ft_export_2(data, commands, x, command_w_e) == -1)
				return (-1);
		free(command_w_e);
	}
	data->res = 1;
	return (0);
}
