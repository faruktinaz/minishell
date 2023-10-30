/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 01:04:15 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/26 00:35:12 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_error(char *str)
{
	printf("%s\n", str);
	g_data.error_flag = 0;
}

void	freelizer(t_arg **line, t_newlst **line2)
{
	while (line && (*line) != NULL)
	{
		free((*line)->content);
		free(*line);
		(*line) = (*line)->next;
	}
	while (line2 &&(*line2) != NULL)
	{
        free_commands((*line2)->content);
		free((*line2)->type);
		free(*line2);
		(*line2) = (*line2)->next;
	}
}

int	is_check(char c)
{
	if (g_data.quot == 0 && (c == '\"' || c == '\''))
		g_data.quot_type = c;
	if (c == g_data.quot_type)
		g_data.quot += 1;
	if (g_data.quot % 2 == 0)
	{
		g_data.quot_type = 1000;
		g_data.quot = 0;
	}
	return (g_data.quot);
}

int	env_control(char *str, int i)
{
	while (str[i] != '\0')
	{
		is_check(str[i]);
		if (g_data.quot_type != '\'' && str[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

void	parse_error(int error_code)
{
	g_data.error_flag = -1;
	g_data.error_code = error_code;
}