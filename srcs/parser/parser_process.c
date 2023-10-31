/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/30 12:53:36 by segurbuz         ###   ########.fr       */
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
			if (ms_isprint(str, i) && (is_oparators(str, i, '|', 0) \
				&& is_oparators(str, i, '>', 0) && is_oparators(str, i, '<', 0)))
				ms_lstadd_back(&temp, \
				ms_lstnew(0, ft_substr(str, start, i - start)));
			ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, i, 1)));
			start = i + 1;
		}
		else if (g_data.quot != 1 && (str[i] == '<' || str[i] == '>'))
		{
			if (str[i + 1] == '<' || str[i + 1] == '>')
			{
				if (str[i + 2] == '<' || str[i + 2] == '>')
					counter = 3;
				else
					counter = 2;
			}
			if (ms_isprint(str, i) && (is_oparators(str, i, '|', 0) \
				&& is_oparators(str, i, '>', 0) && is_oparators(str, i, '<', 0)))
				ms_lstadd_back(&temp, \
				ms_lstnew(0, ft_substr(str, start, i - start)));
			ms_lstadd_back(&temp, ms_lstnew(0, ft_substr(str, i, counter)));
			start = i + counter;
			if (counter == 2 || counter == 3)
				i += counter - 1;
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
	int check;
	
	start = 0;
	i = 0;
	check = 0;
	while (str[++i - 1] != '\0')
	{
		if (is_check(str[i]) != 1 && check == 0 && (str[i] == ' ' || !ft_isprint(str[i])))
		{
			splitting_to_add_list(temp, ft_substr(str, start, i - start));
			start = i + 1;
			check = 1;
		}
		else if (str[i] == '#')
			return ;
		else if(check == 1 && ft_isprint(str[i]))
		{
			start = i;
			check = 0;
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
		printf("list: %s;\n",tmp->content);
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

	temp = ft_calloc(sizeof(t_arg), 1);
	temp->next = NULL;
	split_line(temp, ft_strtrim(g_data.line, " "));
	ms_lstadd_back(&temp, NULL);
	tmp = temp;
	temp = temp->next;
	free(tmp);
	//error_check(temp);
	make_sense(&temp);
	type_counter(&temp);
	if (g_data.error_flag != 0 || temp == NULL)
	{
		printf("HATA\n");
		freelizer(&temp, NULL);
		return ;
	}
	struct_initilaize(NULL, 0);
	find_env_name(temp);
	check_quot_list(temp);
	change_list(temp);
	g_data.list = temp;
}
