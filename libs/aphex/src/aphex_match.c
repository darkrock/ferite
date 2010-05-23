/* borrwed from eggdrop */

/* The quoting character -- what overrides wildcards (do not undef)    */
#define QUOTE '\\'

/* The "matches ANYTHING" wildcard (do not undef)                      */
#define WILDS '*'

/* The "matches ANY NUMBER OF NON-SPACE CHARS" wildcard (do not undef) */
#define WILDP '%'

/* The "matches EXACTLY ONE CHARACTER" wildcard (do not undef)         */
#define WILDQ '?'

#undef tolower
#define tolower(c) tolowertab[c]
static unsigned char tolowertab[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
      0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
      0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x40, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
      'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
      'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
      'x', 'y', 'z', 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
      0x60, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
      'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
      'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
      'x', 'y', 'z', 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
      0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
      0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
      0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
      0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
      0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
      0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
      0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
      0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
      0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
      0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
      0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
      0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
      0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

/* Changing these is probably counter-productive :) */
#define MATCH (match+saved+sofar)
#define NOMATCH 0

/*========================================================================*
 * EGGDROP:   wild_match_per(char *m, char *n)                            *
 * IrcII:     wild_match(char *m, char *n)                                *
 *                                                                        *
 * Features:  Forward, case-insensitive, ?, *, %, ~(optional)             *
 * Best use:  Generic string matching, such as in IrcII-esque bindings    *
 *========================================================================*/
int __aphex_wild_match( unsigned char *m, unsigned char *n)
{
    unsigned char *ma = m, *lsm = 0, *lsn = 0, *lpm = 0, *lpn = 0;
    int match = 1, saved = 0;
    register unsigned int sofar = 0;

  /* take care of null strings (should never match) */
    if ( ( m == 0 ) || ( n == 0 ) || ( !*n ) )
      return NOMATCH;

    while ( *n )
    {
        switch ( *m )
        {
          case 0:
            do
              m--;    /* Search backwards      */
            while ( ( m > ma ) && ( *m == '?' ) );  /* For first non-? char  */
            if ( ( m > ma ) ? ( ( *m == '*' ) && ( m[-1] != QUOTE ) ) : ( *m == '*' ) )
              return MATCH;  /* nonquoted * = match   */
            break;
          case WILDP:
            while (*(++m) == WILDP);  /* Zap redundant %s      */
            if (*m != WILDS)
            {
      /* Don't both if next=*  */
                if( *n != ' ' )
                {  /* WILDS can't match ' ' */
                    lpm = m;
                    lpn = n;  /* Save % fallback spot  */
                    saved += sofar;
                    sofar = 0;  /* And save tally count  */
                }
                continue;  /* Done with %           */
            }
      /* FALL THROUGH */
          case WILDS:
            do
              m++;    /* Zap redundant wilds   */
            while ( ( *m == WILDS ) || ( *m == WILDP ) );
            lsm = m;
            lsn = n;
            lpm = 0;    /* Save * fallback spot  */
            match += (saved + sofar);  /* Save tally count      */
            saved = sofar = 0;
            continue;    /* Done with *           */
          case WILDQ:
            m++;
            n++;
            continue;    /* Match one char        */
          case QUOTE:
            m++;    /* Handle quoting        */
        }

        if( tolower( *m ) == tolower( *n ) )
        {  /* If matching           */
            m++;
            n++;
            sofar++;
            continue;    /* Tally the match       */
        }
        if( lpm )
        {    /* Try to fallback on %  */
            n = ++lpn;
            m = lpm;
            sofar = 0;    /* Restore position      */
            if ( ( *n | 32 ) == 32 )
              lpm = 0;    /* Can't match 0 or ' '  */
            continue;    /* Next char, please     */
        }
        if( lsm )
        {    /* Try to fallback on *  */
            n = ++lsn;
            m = lsm;    /* Restore position      */
      /* Used to test for (!*n) here but it wasn't necessary so it's gone */
            saved = sofar = 0;
            continue;    /* Next char, please     */
        }
        return NOMATCH;    /* No fallbacks=No match */
    }
    while ( ( *m == WILDS ) || ( *m == WILDP ) )
      m++;      /* Zap leftover %s & *s  */
    return ( *m ) ? NOMATCH : MATCH;  /* End of both = match   */
}
