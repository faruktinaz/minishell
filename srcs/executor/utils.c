
void	set_envp(t_exec *data, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	data->env_p = calloc(1024, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		data->env_p[i] = ft_strdup(envp[i]);
		i++;
	}
	data->env_p[i] = NULL;
}