#include "../header/ft_traceroute.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*relais;

	relais = (char *) s;
	while (n > 0)
	{
		*relais = c;
		relais++;
		n--;
	}
	return (s);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (src)
	{
		while (src[i] != '\0')
		{
			dest[j] = src[i];
			i++;
			j++;
		}
		dest[j] = '\0';
	}
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = (char)s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*relais;
	char	*r_src;
	int		i;

	if (!dest && !src)
		return (NULL);
	r_src = (char *) src;
	relais = (char *) dest;
	i = 0;
	while (n > 0)
	{
		relais[i] = r_src[i];
		n--;
		i++;
	}
	return (dest);
}