/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/02 05:02:38 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	split_line(t_arg *temp, char *str)
{
	int	i;
	int	start;
	int	check;

	start = 0;
	i = -1;
	check = 0;
	while (++i < (int)ft_strlen(str) + 1)
	{
		if (is_check(str[i]) != 1 && check == 0 && (str[i] == ' ' \
			|| !ft_isprint(str[i]) || str[i] == '#'))
		{
			splitting_to_add_list(temp, ft_substr(str, start, i - start));
			start = i + 1;
			check = 1;
		}
		else if (check == 1 && ft_isprint(str[i]) && str[i] != ' ')
		{
			start = i;
			check = 0;
		}
		if (str[i] == '#')
			break ;
	}
	free(str);
}

void	type_counter(t_arg	**lst)
{
	t_arg	*list;

	list = *lst;
	g_data.counter = ft_calloc(sizeof(t_counter), 1);
	while (list != NULL)
	{
		if (list->type == INPUT_RDR || list->type == OUTPUT_RDR \
				|| list->type == DOUBLE_OUTPUT_RDR)
			g_data.counter->rdr++;
		else if (list->type == WORD)
			g_data.counter->word++;
		else if (list->type == DOUBLE_INPUT_RDR)
			g_data.counter->heredoc++;
		else if (list->type == PIPE)
			g_data.counter->pipe++;
		list = list->next;
	}
}

void	test(t_arg **temp)
{
	t_arg	*tmp;

	tmp = *temp;
	while (tmp != NULL)
	{
		printf("list: %s;\n",tmp->content);
		tmp = tmp->next;
	}
}

void	ft_parse(void)
{
	t_arg	*temp;
	t_arg	*tmp;

	temp = ft_calloc(sizeof(t_arg), 1);
	temp->next = NULL;
	split_line(temp, ft_strtrim(g_data.line, " "));
	ms_lstadd_back(&temp, NULL);
	tmp = temp;
	temp = temp->next;
	free(tmp);
	make_sense(&temp);
	type_counter(&temp);
	if (error_check(temp) == 1)
	{
		freelizer(&temp, NULL);
		return ;
	}
	find_env_name(temp);
	struct_initilaize(NULL, 0);
	change_list(temp);
	check_quot_list(&g_data.arg);
	g_data.list = temp;
}
