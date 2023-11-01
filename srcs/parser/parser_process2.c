/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 01:19:36 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/01 01:34:11 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_oparators(char *str, int i, int oparator, int rule)
{
	int	check;

	check = 0;
	if (i > 0)
		check = 1;
	if (rule == 0)
	{
		if (str[i - check] != oparator)
			return (1);
	}
	else if (rule == 1)
	{
		if (str[i - check] == oparator)
			return (1);
	}
	return (0);
}

int	ms_isprint(char *str, int i)
{
	int	check;

	check = 0;
	if (i > 0)
		check = 1;
	if (str[i - check] >= 32 && str[i - check] <= 126)
		return (1);
	return (0);
}

void	redirections_parse(t_arg *temp, int *i, int *counter, int *start)
{
	if (g_data.parse_str[(*i) + 1] == '<' || g_data.parse_str[(*i) + 1] == '>')
	{
		if (g_data.parse_str[(*i) + 2] == '<' \
			|| g_data.parse_str[(*i) + 2] == '>')
			(*counter) = 3;
		else
			(*counter) = 2;
	}
	if (ms_isprint(g_data.parse_str, (*i)) \
		&& (is_oparators(g_data.parse_str, (*i), '|', 0) \
		&& is_oparators(g_data.parse_str, (*i), '>', 0) \
		&& is_oparators(g_data.parse_str, (*i), '<', 0)))
		ms_lstadd_back(&temp, \
		ms_lstnew(0, ft_substr(g_data.parse_str, (*start), (*i) - (*start))));
	ms_lstadd_back(&temp, \
		ms_lstnew(0, ft_substr(g_data.parse_str, (*i), (*counter))));
	(*start) = (*i) + (*counter);
	if ((*counter) == 2 || (*counter) == 3)
		(*i) += (*counter) - 1;
	(*counter) = 1;
}

void	pipe_parse(t_arg *temp, int *i, int *start)
{
	if (ms_isprint(g_data.parse_str, (*i)) \
		&& (is_oparators(g_data.parse_str, (*i), '|', 0) \
		&& is_oparators(g_data.parse_str, (*i), '>', 0) \
		&& is_oparators(g_data.parse_str, (*i), '<', 0)))
		ms_lstadd_back(&temp, \
		ms_lstnew(0, ft_substr(g_data.parse_str, (*start), (*i) - (*start))));
	ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(g_data.parse_str, (*i), 1)));
	(*start) = (*i) + 1;
}

void	splitting_to_add_list(t_arg *temp, char *str)
{
	int		i;
	int		start;
	int		counter;

	i = -1;
	start = 0;
	counter = 1;
	g_data.parse_str = str;
	while (str[++i] != '\0')
	{
		if (is_check(str[i]) != 1 && str[i] == '|')
			pipe_parse(temp, &i, &start);
		else if (g_data.quot != 1 && (str[i] == '<' || str[i] == '>'))
			redirections_parse(temp, &i, &counter, &start);
	}
	if (str[ft_strlen(str) - 1] != '|'
		&& str[ft_strlen(str) - 1] != '>' \
		&& str[ft_strlen(str) - 1] != '<')
		ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, start, i - start)));
	free(str);
}
