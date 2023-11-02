/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_built_in.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:26:44 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 08:45:50 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_set_export(t_exec *data, char *export)
{
	int	i;

	i = 0;
	while (data->env_p[i])
		i++;
	data->env_p[i] = malloc(sizeof(char) * ft_strlen(export) + 1);
	ft_strlcpy(data->env_p[i], export, ft_strlen(export) + 1);
}

int	ft_change_dir(t_exec *data, char *token)
{
	char	*t_pwd;
	int		i_pwd;

	t_pwd = malloc(sizeof(char) * 1024);
	if (chdir(token) == -1)
		return (free(t_pwd), -1);
	else
	{
		if (!getcwd(t_pwd, 1024))
			perror("error");
		else
		{
			i_pwd = find_env_dir(data->env_p, "PWD");
			data->new_pwd = ft_strjoin("PWD=", t_pwd);
			if (i_pwd == -1)
				data->old_pwd = ft_strjoin("OLDPWD=", " ");
			else
				data->old_pwd = ft_strjoin("OLDPWD=", data->env_p[i_pwd] + 4);
			ft_exc_chdir(data, i_pwd);
			free(data->new_pwd);
			free(data->old_pwd);
		}
	}
	return (free(t_pwd), 0);
}

void	ft_exc_chdir(t_exec *data, int i_pwd)
{
	if (i_pwd != -1)
		ft_strlcpy(data->env_p[i_pwd], data->new_pwd, \
				ft_strlen(data->new_pwd) + 1);
	ft_strlcpy(data->env_p[find_env_dir(data->env_p, "OLDPWD")], \
				data->old_pwd, ft_strlen(data->old_pwd) + 1);
}

int	find_env_dir(char **env_p, char *find)
{
	int	j;

	j = 0;
	if (find)
	{
		while (env_p[j])
		{
			if (ft_strncmp(env_p[j], find, ft_strlen(find)) == 0)
				return (j);
			else
				j++;
		}
	}
	return (-1);
}

int	check_is_dir(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == '/')
			res = 1;
		i++;
	}
	return (res);
}
