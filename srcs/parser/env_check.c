/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:38:56 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/18 12:54:33 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	env_check(char const *str, char c, int rule)
{
	int	i;

	i = -1;
	if (rule == 1)
	{
		if (ft_isdigit(c) || ft_isalnum(c) || c == '_' \
			|| c == '?')
			return (true);
		return (false);
	}
	else
	{
		while (str[++i] != '\0')
			if (str[i] == '$')
				return (true);
		return (false);
	}
}

int	env_size(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i);
}

char	*env_find(char *path)
{
	int	i;

	i = -1;
	if (ft_strlen(path + 1) == 1)
	{
		if (path[1] == '?')
			return (ft_itoa(g_data.error_code));
	}
	while (g_data.envp[++i])
	{
		if (ft_strncmp(g_data.envp[i], path + 1, ft_strlen(path + 1)) == 0
			&& env_size(g_data.envp[i]) <= (int)ft_strlen(path + 1))
			break ;
	}
	if (g_data.envp[i] == NULL)
		return ("\0");
	else
		return (g_data.envp[i] + env_size(path));
}

char	*env_add_dollars(char *str, char *path)
{
	char	*str_start;
	char	*str_end;
	char	*new_str;
	int		i;

	i = 0;
	struct_initilaize(NULL, 0);
	i = env_control(str, i);
	str_start = ft_substr(str, 0, i);
	str_end = ft_substr(str, (ft_strlen(path + 1) + i) + 1, ft_strlen(str) - i);
	i = -1;
	while (str[++i] != '\0')
	{
		if (g_data.quot_type != '\'' && str[i] == '$')
		{
			path = env_find(path);
			g_data.error_code = 0;
			break ;
		}
	}
	new_str = ms_strjoin(str_start, path);
	new_str = ms_strjoin(new_str, str_end);
	free(str);
	return (free(str_end), new_str);
}

void	find_env_name(t_arg *temp)
{
	int		i;
	int		len;
	char	*path;

	while (temp != NULL)
	{
		if (env_check(temp->content, '\0', 0))
		{
			i = -1;
			while (temp->content[++i] != '\0')
			{
				is_check(temp->content[i]);
				if (g_data.quot_type != '\'' && temp->content[i - 1] == '$')
				{
					len = 1;
					while (len++ && env_check(NULL, temp->content[i], 1))
					{
						if (!env_check(NULL, temp->content[i], 1)
							|| temp->content[i - 1] == '?')
							break ;
						i++;
					}
					path = ft_substr(temp->content, (i - len) + 1, len - 1);
					if ((path[ft_strlen(path) - 1] == '?')
						&& ft_strlen(path) >= 3)
						path[ft_strlen(path) - 1] = '\0';
					temp->content = env_add_dollars(temp->content, path);
					g_data.quot = 0;
					i = -1;
					free(path);
				}
			}
		}
		temp = temp->next;
	}
}
