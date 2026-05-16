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

/* Filter print: both strings are exactly FILTER_W (7) chars wide */
# define FILTER_FILL    "#######"
# define BRAND_WINSTON  "WINSTON"

/* Coffee cup (drawn to the left of the cigarette). Body 8 wide with
** CUP_H interior rows; coffee (brown '~') fills from the bottom and the
** level drops over time, in sync with the cigarette burning down.
** No handle (a handle made it look like a beer mug). */
# define CUP_W      8
# define CUP_GAP    12
# define CUP_H      4
# define CUP_TOP    " .------. "
# define CUP_BOT    " `------' "
# define CUP_LIQ    "~~~~~~"
# define CUP_EMPTY  "      "
# define CUP_COLOR  "\033[38;5;130m"

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
