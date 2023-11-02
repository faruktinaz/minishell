/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:25:46 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 01:46:19 by ogenc            ###   ########.fr       */
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

int	ft_ex_c(char **commands, int x)
{
	if (commands[x][0] >= '0' && commands[x][0] <= '9')
	{
		printf("export: not an identifier: %s\n", commands[x]);
		g_data.error_code = 256;
		return (-1);
	}
	return (0);
}

int	ft_export_2(t_exec *data, char **commands, int x)
{
	int	j;

	j = 0;
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
	return (0);
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
		if (ft_ex_c(commands, x) == -1)
			return (-1);
		j = 0;
		if (command_w_eq && find_env_dir(data->env_p, command_w_eq) != -1)
			ft_strlcpy(data->env_p[find_env_dir(data->env_p, command_w_eq)], \
					commands[x], ft_strlen(commands[x]) + 1);
		else
			if (ft_export_2(data, commands, x) == -1)
				return (-1);
		free(command_w_eq);
		x++;
	}
	data->res = 1;
	return (0);
}
