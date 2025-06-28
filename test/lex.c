#include "../src/lex.c"

int main(int argc, char **argv)
{
	t_token	*token;
	if (argc != 2)
		return (1);
	token = lex(argv[1]);
	print_tokens(token);
	free_token(token);
	return (0);
}
