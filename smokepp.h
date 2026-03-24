/* ************************************************************************** */
/*                                                                            */
/*   smokepp.h - ASCII art and constants for smoke++                          */
/*   Inspired by sl(1) by Toyoda Masashi                                      */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMOKEPP_H
# define SMOKEPP_H

/* Cigarette geometry */
# define EMBER_W      2
# define FILTER_W     7
# define PAPER_INIT   36

/* Animation timing */
# define FRAME_US     40000   /* 40ms per frame, ~25fps */
# define BURN_RATE_FAST 7     /* ~10 sec total (36 * 7 * 40ms) */
# define BURN_RATE_SLOW 100   /* ~2.5 min total (36 * 100 * 40ms) */
# define STUB_FRAMES  80      /* frames to show the filter stub */
# define STUB_FRAMES_FAST 25  /* shorter stub for quick mode */

/* Smoke particle system */
# define MAX_SMOKE    120
# define SMOKE_LIFE   30

/* Ember flicker patterns */
# define EMBER_A      "oO"
# define EMBER_B      "Oo"
# define EMBER_DIM    ".."

/* Ash */
# define ASH_LIFE     20

#endif
