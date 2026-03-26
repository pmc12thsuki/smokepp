/* ************************************************************************** */
/*                                                                            */
/*   smokepp.c - A burning cigarette for your terminal                        */
/*   Usage: smoke++ [-all]                                                    */
/*   Inspired by sl(1) by Toyoda Masashi                                      */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "smokepp.h"

static int	g_mode_all;
static int	g_rows;
static int	g_cols;

typedef struct s_smoke
{
	int	y;
	int	x;
	int	age;
	int	seed;
	int	alive;
}	t_smoke;

typedef struct s_ash
{
	int	y;
	int	x;
	int	age;
	int	active;
}	t_ash;

static t_smoke			g_smk[MAX_SMOKE];
static t_ash			g_ash;
static int				g_frame;
static struct termios	g_orig_term;

/*
** Terminal helpers using ANSI escape sequences.
*/
static void	term_move(int y, int x)
{
	printf("\033[%d;%dH", y + 1, x + 1);
}

static void	term_clear(void)
{
	printf("\033[2J");
}

static void	term_color_on(void)
{
	printf("\033[31m");
}

static void	term_color_off(void)
{
	printf("\033[0m");
}

static void	get_term_size(void)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
	{
		g_rows = ws.ws_row;
		g_cols = ws.ws_col;
	}
	else
	{
		g_rows = 24;
		g_cols = 80;
	}
}

/*
** Returns the character to draw for a smoke particle at a given age.
** Progression: wisp -> forming -> thick -> thinning -> gone
*/
static char	smoke_char(int age)
{
	if (age < 2)
		return ('~');
	if (age < 5)
		return ('*');
	if (age < 9)
		return ('o');
	if (age < 14)
		return ('O');
	if (age < 18)
		return ('o');
	if (age < 22)
		return ('.');
	if (age < 26)
		return ('\'');
	return (' ');
}

/*
** Safe putchar at position: only draw if within terminal bounds.
*/
static void	safe_addch(int y, int x, char c)
{
	if (y >= 0 && y < g_rows && x >= 0 && x < g_cols)
	{
		term_move(y, x);
		putchar(c);
	}
}

/*
** Safe string draw, skipping out-of-bounds positions.
*/
static void	my_mvaddstr(int y, int x, const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		safe_addch(y, x + i, s[i]);
		i++;
	}
}

/*
** Spawn a new smoke particle at position (y, x).
*/
static void	spawn_smoke(int y, int x)
{
	int	i;

	i = 0;
	while (i < MAX_SMOKE)
	{
		if (!g_smk[i].alive)
		{
			g_smk[i].y = y;
			g_smk[i].x = x;
			g_smk[i].age = 0;
			g_smk[i].seed = g_frame + i;
			g_smk[i].alive = 1;
			return ;
		}
		i++;
	}
}

/*
** Update all smoke particles: age, move, draw.
** Young smoke rises fast (every frame), older smoke slows down.
** Drift is deterministic per-particle using seed for variety.
*/
static void	update_and_draw_smoke(void)
{
	int		i;
	int		drift_period;
	int		d;
	char	ch;

	i = -1;
	while (++i < MAX_SMOKE)
	{
		if (!g_smk[i].alive)
			continue ;
		g_smk[i].age++;
		if (g_smk[i].age >= SMOKE_LIFE)
		{
			g_smk[i].alive = 0;
			continue ;
		}
		/* Rise: fast when young, slower when old */
		if (g_smk[i].age < 8 || g_smk[i].age % 2 == 0)
			g_smk[i].y--;
		/* Lateral drift: each particle has its own rhythm */
		drift_period = 3 + (g_smk[i].seed % 3);
		if (g_smk[i].age % drift_period == 0)
		{
			d = (g_smk[i].seed + g_smk[i].age / drift_period) % 3 - 1;
			g_smk[i].x += d;
		}
		/* Draw */
		ch = smoke_char(g_smk[i].age);
		if (ch != ' ')
			safe_addch(g_smk[i].y, g_smk[i].x, ch);
	}
}

/*
** Draw the cigarette at position (cy, cx) with given paper length.
**
**   ,-----------------------------------------------.
**   |oO                                      |#####|
**   `-----------------------------------------------'
*/
static void	draw_cigarette(int cy, int cx, int paper_len)
{
	int			total;
	int			i;
	const char	*ember;

	total = EMBER_W + paper_len + FILTER_W;
	/* Top border */
	safe_addch(cy, cx, ',');
	i = -1;
	while (++i < total)
		safe_addch(cy, cx + 1 + i, '-');
	safe_addch(cy, cx + total + 1, '.');
	/* Body */
	safe_addch(cy + 1, cx, '|');
	/* Ember (flickers) */
	if (paper_len > 2)
		ember = (g_frame / 6 % 2) ? EMBER_A : EMBER_B;
	else
		ember = EMBER_DIM;
	my_mvaddstr(cy + 1, cx + 1, ember);
	/* Paper (blank space) */
	i = -1;
	while (++i < paper_len)
		safe_addch(cy + 1, cx + 1 + EMBER_W + i, ' ');
	/* Separator and filter */
	safe_addch(cy + 1, cx + 1 + EMBER_W + paper_len, '|');
	i = -1;
	while (++i < FILTER_W)
		safe_addch(cy + 1, cx + 2 + EMBER_W + paper_len + i, '#');
	safe_addch(cy + 1, cx + total + 1, '|');
	/* Bottom border */
	safe_addch(cy + 2, cx, '`');
	i = -1;
	while (++i < total)
		safe_addch(cy + 2, cx + 1 + i, '-');
	safe_addch(cy + 2, cx + total + 1, '\'');
}

/*
** Draw the filter stub after the cigarette has fully burned.
*/
static void	draw_stub(int cy, int cx)
{
	int	i;

	safe_addch(cy, cx, ',');
	i = -1;
	while (++i < FILTER_W)
		safe_addch(cy, cx + 1 + i, '-');
	safe_addch(cy, cx + FILTER_W + 1, '.');
	safe_addch(cy + 1, cx, '|');
	i = -1;
	while (++i < FILTER_W)
		safe_addch(cy + 1, cx + 1 + i, '#');
	safe_addch(cy + 1, cx + FILTER_W + 1, '|');
	safe_addch(cy + 2, cx, '`');
	i = -1;
	while (++i < FILTER_W)
		safe_addch(cy + 2, cx + 1 + i, '-');
	safe_addch(cy + 2, cx + FILTER_W + 1, '\'');
}

/*
** Update and draw falling ash particle.
*/
static void	update_ash(void)
{
	if (!g_ash.active)
		return ;
	g_ash.age++;
	if (g_ash.age > ASH_LIFE)
	{
		g_ash.active = 0;
		return ;
	}
	g_ash.y += (g_ash.age % 3 == 0) ? 1 : 0;
	g_ash.x += (g_ash.age % 5 == 0) ? 1 : 0;
	safe_addch(g_ash.y, g_ash.x, (g_ash.age % 2) ? '.' : ',');
}

/*
** Trigger ash falling from the ember position.
*/
static void	drop_ash(int cy, int cx)
{
	g_ash.y = cy + 3;
	g_ash.x = cx + 1;
	g_ash.age = 0;
	g_ash.active = 1;
}

/*
** Set terminal to raw mode (no echo, no canonical, no blocking).
*/
static void	init_display(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &g_orig_term);
	raw = g_orig_term;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	printf("\033[?25l");
	get_term_size();
}

/*
** Restore terminal to original state.
*/
static void	cleanup_display(void)
{
	printf("\033[0m");
	printf("\033[?25h");
	term_clear();
	term_move(0, 0);
	fflush(stdout);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_term);
}

/*
** Parse arguments. Returns 0 on success, 1 on error.
** -a / --addict : full burn mode (~2.5 min, SIGINT ignored)
*/
static int	parse_args(int argc, char *argv[])
{
	int	i;

	g_mode_all = 0;
	i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i], "-a") == 0
			|| strcmp(argv[i], "--addict") == 0)
			g_mode_all = 1;
		else
		{
			write(STDERR_FILENO,
				"Usage: smoke++ [-a | --addict]\n", 30);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	int	paper_len;
	int	cy;
	int	right_edge;
	int	total;
	int	cx;
	int	i;
	int	sx;
	int	burn_rate;
	int	stub_frames;

	if (parse_args(argc, argv))
		return (1);
	burn_rate = g_mode_all ? BURN_RATE_SLOW : BURN_RATE_FAST;
	stub_frames = g_mode_all ? STUB_FRAMES : STUB_FRAMES_FAST;
	paper_len = PAPER_INIT;
	g_frame = 0;
	memset(g_smk, 0, sizeof(g_smk));
	memset(&g_ash, 0, sizeof(g_ash));
	/* -all mode: you must watch it burn. No escape. */
	if (g_mode_all)
		signal(SIGINT, SIG_IGN);
	init_display();
	cy = g_rows * 2 / 3;
	total = EMBER_W + PAPER_INIT + FILTER_W + 2;
	right_edge = (g_cols + total) / 2;
	/* Main burn loop */
	while (paper_len > 0)
	{
		term_clear();
		total = EMBER_W + paper_len + FILTER_W + 2;
		cx = right_edge - total;
		/* Draw cigarette with colored ember */
		term_color_on();
		draw_cigarette(cy, cx, paper_len);
		term_color_off();
		/* Spawn smoke above the ember */
		sx = cx + 1 + (g_frame % 3) - 1;
		spawn_smoke(cy - 1, sx);
		if (g_frame % 2 == 0)
			spawn_smoke(cy - 1, sx + 1);
		update_and_draw_smoke();
		update_ash();
		/* Burn down the paper */
		if (g_frame > 0 && g_frame % burn_rate == 0)
		{
			paper_len--;
			drop_ash(cy, cx);
		}
		fflush(stdout);
		usleep(FRAME_US);
		g_frame++;
	}
	/* Show the filter stub until smoke clears */
	cx = right_edge - (FILTER_W + 2);
	i = 0;
	while (i < stub_frames)
	{
		term_clear();
		draw_stub(cy, cx);
		update_and_draw_smoke();
		update_ash();
		fflush(stdout);
		usleep(FRAME_US);
		g_frame++;
		i++;
	}
	cleanup_display();
	return (0);
}
