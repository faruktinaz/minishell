/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_ec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 01:06:22 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 01:06:51 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo_3(char **commands, int i, int x)
{
	if (x == 0)
	{
		printf("%s", commands[i]);
		if (commands[i + 1])
			printf(" ");
		g_data.n_added = 1;
	}
	else if (x == 1)
	{
		printf("%s", commands[i]);
		if (commands[i + 1])
			printf(" ");
	}
}

int	ft_echo_2(char **commands, int i)
{
	int	j;

	while (commands[i])
	{
		if (!ft_strncmp(commands[i], "-n", 2) && !g_data.n_added)
		{
			j = 2;
			while (commands[i][j])
			{
				if (commands[i][j] != 'n')
				{
					ft_echo_3(commands, i, 0);
					break ;
				}
				else
					break ;
			}
		}
		else
			ft_echo_3(commands, i, 1);
		i++;
	}
	return (i + 1);
}

void	ft_echo_4(char **commands)
{
	int	j;

	j = 2;
	while (commands[1][j])
	{
		if (commands[1][j] != 'n')
			g_data.b_check = 0;
		j++;
	}
	g_data.b_check = 1;
}

void	ft_echo(char **commands)
{
	int	i;

	i = 1;
	g_data.b_check = 2;
	g_data.n_added = 0;
	if (commands[1] && !ft_strncmp(commands[1], "-n", 2))
		ft_echo_4(commands);
	if (commands[1] && g_data.b_check == 1)
		i = ft_echo_2(commands, i);
	else
	{
		while (commands[i])
		{
			ft_echo_3(commands, i, 1);
			i++;
		}
		printf("\n");
	}
	g_data.error_code = 0;
}
