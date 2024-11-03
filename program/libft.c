#include "../includes/ft_traceroute.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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