
int expand_pos_param(char **ret, char **str, int argc, char **argv)
{
	char	tmp;
	int		i;
	size_t	varlen;

	*str++;
	i = 0;
	while (**str && is_numeric(**str))
		i++;
	tmp = *str[i];
	*str[i] = 0; // null-terminate the position string.
	varlen = strlen(*str) + 1; // one more for the '$'
	pos = atoi(*str);
	str[i] = tmp;
	if (pos >= argc)
		return (-varlen);
	strcpy(*ret, argv[pos]);
	return (strlen(argv[pos]) - varlen);

/* @brief copy the value of the variable pointed to in str to ret and advance both
 * @param ret a p2ptr to the beginning of the expanded string. if *ret == NULL,
 * 	just return the difference between the value and varname lengths (including '$')
 * @param str a p2ptr to the '$' character before the variable name
 */
int	expand_variable(char **ret, char **str, int	argc, char **argv)
{
	char	tmp;
	int		i;
	char	*val;
	size_t	varlen;
	
	if (is_numeric(*(*str + 1))) // first character is numeric
		return (expand_pos_param(ret, str, argc, argv))
	*str++;
	i = 0;
	while (**str && (is_alpha(**str) || is_numeric(**str) || **str == '_'))
		i++;
	tmp = *str[i];
	*str[i] = 0; // null-terminate the variable name
	varlen = strlen(*str) + 1; // one more for the '$'
	val = getenv(*str);
	*str[i] = tmp;
	if (!val) // NULL env vars get expanded to empty string
		return (-strlen(varname) - 1);
	if (*ret)
	{
		strcpy(*ret, val);
		*ret += strlen(val);
	}
	return (strlen(val) - varlen);
}

/* @brief get the (length of the) new string with expanded variables
 * @param str string with variables
 * @param len length of new string.  if this is 0, just return the length of the 
 *  expanded string
 * NOTE: string must not end with '\'!
 */
static size_t count_or_expand(char **str, size_t len)
{
	int 	quoted;
	char	*ret;
	
	ret = NULL;
	if (len)
	{
		ret = malloc(len);
		if (!ret)
		{
			free (*str);
			*str = NULL;
			return (0);
		}
	}
	quoted = false;
	while (**str)
	{
		if (**str == '\\')
			*ret++ = *(*str)++;
		else if (**str == '\'')
			quoted = !quoted;
		else if (**str == '$')
		{
			len += expand_variable(&ret, str);
			continue;
		}
		*ret++ = *(*str)++;
	}
	if (len)
	{
		free(str);
		*str = ret;
	}
	return (len);
}

/* a valid input string does not end with '\' and has no unclosed quotes
 * and no unclosed unquoted parentheses
 */
bool	is_valid_input(char *str)
{
	bool	quoted;
	bool	qquoted;
	int		level;

	quoted = false;
	qquoted = false;
	level = 0;
	while (*str)
	{
		if (*str == '"' && !quoted)
			qquoted = !qquoted;
		if (*str == '\'' && !qquoted)
			quoted = !quoted;
		if (*str == '\\' && !quoted)
		{
			if (!*(str + 1)) // escaped NULL terminator
				return (false);
			str += 2;
			continue;
		}
		if (*str == '{' && (!quoted && !qquoted))
			level++;
		if (*str == '}' && (!quoted && !qquoted))
			level--;
		if (level < 0)
			return (false);
		str++;
	}
	if (level != 0 || (quoted || qquoted)) // invalid parens or unclosed quote
		return (false);
	return (true);
}

char *expand_variables(char *str)
{
	int	len;

	if (!*str) // str is empty
		return (str);
	if (!is_valid_input(str))
	{
		free(str);
		return (NULL);
	}
	len = count_or_expand(&str, 0);
	count_or_expand(&str, len);
	return (str);
}
