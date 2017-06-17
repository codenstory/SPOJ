/*
 * AUTHOR: Serikzhan Kazi
 *         Helsingin Yliopisto
 *
 * NOTES:
 * DAWG with dynamically-allocated memory
 * Good thing about this: since DAWG construction
 * is online, we don't need to store the text itself, rather
 * read it char-by-char via getchar().
 * Also, it's proven via amortized analysis that
 * dynamic table that doubles its size when
 * demand exceeds its current maxcapacity
 * incurs no cost overall. So we use
 * such dynamic tables to store all our information:
 * suffix pointers-array, ``edges'', etc.
 * All the comments below are actually paraphrases
 * of the reference-paper's pseudo-code.
 *
 * REFERENCE: 
 *          A.Blumer, J.Blumer, D.Haussler, A.Ehrenfeucht, M.T.Chen, J.Seiferas, 
 *          ``The smallest automaton recognizing the subwords of a text'' (1985)
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TST(u,k) (1 & ((u)>>(k)))           /* TEST k-th bit of a bitmask ``u''*/
#define SET(u,k) ((u) |= (1UL << (k)))      /* SET k-th bit of a bitmask ``u'' */
enum { PRIMARY, SECONDARY };
#define UNDEF (-1)
#define A 26
#define QUITO(where) { printf("memory allocation failed in %d",where); assert(0); }
#define max(x,y)  ( (x) < (y) ? (y) : (x) )

int source,n,*sufptr,**e;
unsigned int *arc,*is_prim,m,*d;

int new_vertex() {
    int v = n++;
    /*
     * if demand exceeds current capacity, double the size
     * of all the tables
     */
    if ( n > m ) {
        m = ((m << 1) < 2 ? 2 : (m << 1));
        if ( !(arc = (unsigned int *)realloc( arc, m*sizeof 0[arc] )) )
           QUITO("new_vertex");
        if ( !(is_prim = (unsigned int *)realloc( is_prim, m*sizeof 0[is_prim] )) )
           QUITO("new_vertex");
        if ( !(sufptr = (int *)realloc( sufptr, m*sizeof 0[sufptr] ))) 
           QUITO("new_vertex");
        if ( !(e = (int **)realloc(e, m*sizeof **e)) )
            QUITO("new_vertex");
        if ( !(d = (unsigned int *)realloc(d, m*sizeof *d)) )
            QUITO("new_vertex");
    }
    /*
     * new-born vertex does not have any transitions: arc[v] = 0
     */
    arc[v] = is_prim[v] = d[v] = 0;
    if ( !(e[v] = (int *)calloc(A,sizeof *e[v])) )
        QUITO("new_vertex");

    return v;

}

void set_transition( int x, int y, char ch, int typpi ) {
    SET(arc[x],ch-'a');
    if ( typpi == PRIMARY ) {
        SET(is_prim[x],ch-'a');
        d[y] = d[x] + 1;
    }
    e[x][ch-'a'] = y;
}

int split( int parentstate, char ch ) {
    int childstate = e[parentstate][ch-'a'],
        curstate,
        newchildstate = new_vertex();

    /*
     * Redirect ``parentstate'''s ch-transition to
     * point to ``newchildstate'', PRIMARY
     */
    set_transition( parentstate, newchildstate, ch, PRIMARY );
    /*
     * duplicate all the transitions of ``childstate'' in
     * ``newchildstate'' as well, but make them all SECONDARY
     */
    memcpy( e[newchildstate], e[childstate], A*sizeof 0[e[childstate]]  );
    arc[newchildstate] = arc[childstate];

    sufptr[newchildstate] = sufptr[childstate], sufptr[childstate] = newchildstate;

    for ( curstate = parentstate; curstate != source; ) {
        curstate = sufptr[curstate];
        /*
         * If the ``curstate'' has a SECONDARY edge going to ``childstate``,
         * then redirect it to ``newchildstate'', otherwise quit
         */
        if ( TST(arc[curstate],ch-'a') && !TST(is_prim[curstate],ch-'a') && e[curstate][ch-'a'] == childstate )
            e[curstate][ch-'a'] = newchildstate;
        else
            break ;
    }

    return newchildstate;

}

int update( int cursink, char ch ) {
    int newsink,
        curstate,
        sufstate = UNDEF;

    newsink = new_vertex();
    set_transition( cursink, newsink, ch, PRIMARY );

    for ( curstate = cursink; curstate != source && sufstate == UNDEF; ) {
        curstate = sufptr[curstate];
        if ( !TST(arc[curstate],ch-'a') )
            set_transition( curstate, newsink, ch, SECONDARY );
        else if ( TST(is_prim[curstate],ch-'a') )
            sufstate = e[curstate][ch-'a'];
        else
            sufstate = split( curstate, ch );
    }

    /*
     * if ``sufstate'' is still undefined, make it source
     */
    if ( sufstate == UNDEF )
        sufstate = source;
    /*
     * assign ``sufstate'' to the suffix-pointer of ``newsink''
     */
    sufptr[newsink] = sufstate;

    return newsink;

}


void buildDAWG() {
    int cursink,ch;
    for ( n = m = 0, cursink = source = new_vertex(); (ch = getchar()) != EOF && ch != '\n'; )
        cursink = update( cursink, ch );
}

int main() {
    int ch,len,best_len,p;

#ifndef ONLINE_JUDGE
    freopen("lcs2.in","r",stdin);
#endif

    buildDAWG(), sufptr[source] = UNDEF;
    for ( best_len = -1, len = d[p = source]; (ch = getchar()) != EOF && ch != '\n' && (best_len = max(best_len,len)) >= 0; ) {
        if ( !TST(arc[p],ch-'a') ) {
            while ( p != UNDEF && !TST(arc[p],ch-'a') )
                p = sufptr[p];
            if ( p == UNDEF ) {
                p = source, len = 0;
                continue ;
            }
            len = d[p];
        }
        p = e[p][ch-'a'], ++len;
    }
    printf("%d\n",max(best_len,len));

    return 0;
}


