/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:50:02 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 02:22:24 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pipe_check(t_arg *temp)
{
	if (temp->next == NULL)
		return (parse_error(1, "syntax error near unexpected token '|'"), 1);
	else if (ft_strcmp(temp->next->content, "|") == 0)
		return (parse_error(1, "syntax error near unexpected token '|'"), 1);
	return (0);
}

int	rdr_check2(t_arg *temp)
{
	if (ft_strcmp(temp->content, "<><") == 0)
		return (parse_error(258, "syntax error near unexpected token '<'"), 1);
	else if (ft_strcmp(temp->content, "<>>") == 0)
		return (parse_error(258, "syntax error near unexpected token '>'"), 1);
	else if (ft_strcmp(temp->content, "><<") == 0)
		return (parse_error(258, "syntax error near unexpected token '<'"), 1);
	else if (ft_strcmp(temp->content, "><>") == 0)
		return (parse_error(258, "syntax error near unexpected token '>'"), 1);
	else if (temp->next == NULL)
		return (parse_error(258 \
			, "syntax error near unexpected token 'newline'"), 1);
	if (ft_strcmp(temp->next->content, "|") == 0)
		return (parse_error(258, "syntax error near unexpected token '|'"), 1);
	return (0);
}

int	rdr_check(t_arg *temp)
{
	if (ft_strcmp(temp->content, ">>>") == 0)
		return (parse_error(258, "syntax error near unexpected token '>'"), 1);
	else if (ft_strcmp(temp->content, "<<<") == 0)
		return (parse_error(258, "syntax error near unexpected token '<'"), 1);
	else if (ft_strcmp(temp->content, ">><") == 0)
		return (parse_error(258, "syntax error near unexpected token '<'"), 1);
	else if (ft_strcmp(temp->content, "<<>") == 0)
		return (parse_error(258, "syntax error near unexpected token '>'"), 1);
	else if (ft_strcmp(temp->content, "><") == 0)
		return (parse_error(258, "syntax error near unexpected token '<'"), 1);
	else if (ft_strcmp(temp->content, "<>") == 0)
		return (parse_error(258, "syntax error near unexpected token '>'"), 1);
	else
	{
		if (rdr_check2(temp) == 1)
			return (1);
	}
	return (0);
}

int	error_check(t_arg *temp)
{
	int	check;

	check = 0;
	if (g_data.quot == 1 && g_data.quot_type == '\"')
		return (parse_error(1, "syntax error near unexpected token '\"'"), 1);
	if (g_data.quot == 1 && g_data.quot_type == '\'')
		return (parse_error(1, "syntax error near unexpected token '\''"), 1);
	if (temp == NULL)
		return (parse_error(0, NULL), 1);
	else if (ft_strlen(temp->content) < 1)
		return (parse_error(0, NULL), 1);
	else if (ft_strcmp(temp->content, "|") == 0)
		return (parse_error(258, "syntax error near unexpected token '|'"), 1);
	while (temp != NULL)
	{
		if (ft_strcmp(temp->content, "|") == 0)
			check = pipe_check(temp);
		else if (*(temp->content) == '<' || *(temp->content) == '>')
			check = rdr_check(temp);
		if (check == 1)
			return (1);
		temp = temp->next;
	}
	return (0);
}
