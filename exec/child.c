void	child_process(t_pipe_data *data, t_initial_fd *fd, int child_num)
{
	t_fd_dup	dup;
	char		**nargv;

	if (setup_redir(fd -> input_fd, fd -> output_fd, &dup) != 0)
		exit(1);
	nargv = exec_arg(data, child_num);
	if (!nargv)
	{
		perror ("nargv before execution");
		close_fd(dup.input_dup);
		close_fd(dup.output_dup);
		exit(1);
	}
	execution(nargv, data -> envp);
	perror ("Execution failed in child 1");
	free(nargv);
	close_fd(dup.input_dup);
	close_fd(dup.output_dup);
	exit(1);
}

void	child1(t_pipe_data *data)
{
	t_initial_fd	fd;
	int				child_num;

	child_num = 1;
	close_fd(data -> pipefd[0]);
	fd = (t_initial_fd){data->fd_in, data->pipefd[1]};
	child_process(data, &fd, child_num);
}

void	child2(t_pipe_data *data)
{
	t_initial_fd	fd;
	int				child_num;
	int				fd_out;

	child_num = 2;
	close(data -> pipefd[1]);
	close(data -> fd_in);
	fd_out = open(data -> argv[data -> argc - 1], O_WRONLY 
			| O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		perror ("open output file");
		exit (-1);
	}
	fd = (t_initial_fd){data -> pipefd[0], fd_out};
	