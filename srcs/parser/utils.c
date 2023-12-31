/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 01:04:15 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/02 07:38:36 by segurbuz         ###   ########.fr       */
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
	void	*temp_next;

	while (line && (*line) != NULL)
	{
		free((*line)->content);
		temp_next = (*line)->next;
		free(*line);
		(*line) = temp_next;
	}
	while (line2 && (*line2) != NULL)
	{
		free_commands((*line2)->content);
		free((*line2)->type);
		temp_next = (*line2)->next;
		free(*line2);
		(*line2) = temp_next;
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
		if (g_data.quot_type != '\'' && (str[i] == '$' || str[i] == '~'))
			return (i);
		i++;
	}
	return (i);
}

void	parse_error(int error_code, char *str)
{
	if (str != NULL)
	{
		dup2(2, 1);
		printf("minishell: %s\n", str);
		dup2(g_data.default_out, 1);
	}
	g_data.error_flag = -1;
	g_data.error_code = error_code * 256;
}
