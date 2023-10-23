/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/18 13:05:03 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	splitting_to_add_list(t_arg *temp, char *str)
{
	int		i;
	int		start;
	int		counter;

	i = -1;
	start = 0;
	counter = 1;
	while (str[++i] != '\0')
	{
		if (is_check(str[i]) != 1 && str[i] == '|')
		{
			if (ft_isprint(str[i - 1]) && (str[i - 1] != '|' \
				&& str[i - 1] != '>' && str[i - 1] != '<'))
				ms_lstadd_back(&temp, \
				ms_lstnew(0, ft_substr(str, start, i - start)));
			ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, i, 1)));
			start = i + 1;
		}
		else if (g_data.quot != 1 && (str[i] == '<' || str[i] == '>'))
		{
			if (str[i + 1] == '<' || str[i + 1] == '>')
				counter = 2;
			if (ft_isprint(str[i - 1]) && str[i - 1] != '|')
				ms_lstadd_back(&temp, \
				ms_lstnew(0, ft_substr(str, start, i - start)));
			ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, i, counter)));
			start = i + counter;
			if (counter == 2)
				i++;
			counter = 1;
		}
	}
	if (str[ft_strlen(str) - 1] != '|'
		&& str[ft_strlen(str) - 1] != '>' \
		&& str[ft_strlen(str) - 1] != '<')
		ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, start, i - start)));
	free(str);
}

void	split_line(t_arg *temp, char *str)
{
	int	i;
	int	start;

	i = -1;
	start = 0;
	while (++i || 1)
	{
		if (is_check(str[i]) != 1 && (!str[i] || str[i] == ' ') \
		&& !(ft_strlen(str) < 1))
		{
			splitting_to_add_list(temp, ft_substr(str, start, i - start));
			while (str[i] == ' ' || !str[i])
				i++;
			i--;
			start = i + 1;
		}
		else if (str[i] == '#')
			return ;
		if (str[i] == '\0')
		{
			if (g_data.quot == 1)
				g_data.error_flag = -1;
			break ;
		}
	}
	free(str);
}

void	test(t_arg **temp)
{
	t_arg	*tmp;

	tmp = *temp;
	while (tmp != NULL)
	{
		printf("list: %s -> %d;\n",tmp->content , tmp->type);
		tmp = tmp->next;
	}
}

void	type_counter(t_arg	**lst)
{
	t_arg	*list;

	list = *lst;
	g_data.counter = ft_calloc(sizeof(t_counter), 1);
	while (list != NULL)
	{
		if (list->type == INPUT_RDR || list->type == OUTPUT_RDR
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

void	ft_parse(void)
{
	t_arg	*temp;
	t_arg	*tmp;

	temp = malloc(sizeof(t_arg));
	temp->next = NULL;
	split_line(temp, ft_strtrim(g_data.line, " "));
	ms_lstadd_back(&temp, NULL);
	tmp = temp;
	temp = temp->next;
	free(tmp);
	error_check(temp);
	make_sense(&temp);
	type_counter(&temp);
	if (g_data.error_flag != 0)
	{
		printf("HATA\n");
		freelizer(&temp);
		return ;
	}
	struct_initilaize(NULL, 0);
	find_env_name(temp);
	check_quot_list(temp);
	change_list(temp); // leak var
	g_data.list = temp;
	//freelizer(&temp); //leak çözüyor ama segment yediriyor.
}
