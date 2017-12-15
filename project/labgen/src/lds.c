/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: lds.c 433 2017-10-18 20:37:37Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/lds.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "top.h"
#include "lds.h"

/*======================================================================*/
/*= constructor/destructor                                             =*/

extern Tlds * lds_new ()
{
    Tlds* ret = u_malloc0( sizeof(*ret) );
    return ret;
}

extern Tlds * lds_new_lds(const Tlds* lds)
{
    int x,y;
    Tlds* ret = u_malloc( sizeof(*ret) );
    memcpy(ret,lds,sizeof(*ret));
    for (x=0 ; x<lds->dx ; x+=1) 
        for (y=0 ; y<lds->dy ; y+=1) 
            if (ret->squares[x][y].opt==LDS_OptMD)
                ret->squares[x][y].sq_mdp = lds_sqmd_new_sqmd(
                        ret->squares[x][y].sq_mdp);
    return ret;
}

extern void   lds_free(Tlds *ds)
{
    int x,y;
    for (x=0 ; x<ds->dx ; x+=1) 
        for (y=0 ; y<ds->dy ; y+=1) 
            if (ds->squares[x][y].opt==LDS_OptMD)
                lds_sqmd_free( ds->squares[x][y].sq_mdp );
    free( ds );
}

/*======================================================================*/
/*= setters                                                            =*/

extern void lds_size_set(Tlds* ds, int dx, int dy)
{ ds->dx = dx; ds->dy = dy; }


extern void lds_draw_xy (Tlds* ds, TdrawOpt dopt, int x, int y)
{ Tpoint pt = { x, y }; lds_draw_pt(ds,dopt,pt); }

extern void lds_draw_pt (Tlds* ds, TdrawOpt dopt, Tpoint pt)
{
#if ASSERT_ENABLED
    if ( lds_check_pt(ds,pt) ) ASSERT_MESS();
#endif
    switch ( dopt ) {
        case LG_DrawWall:
            ds->squares[pt.x][pt.y].kind=LDS_WALL;
            break;
        case LG_DrawUnwall:
            ds->squares[pt.x][pt.y].kind=LDS_FREE;
            break;
        case LG_DrawToggle:
            if ( ds->squares[pt.x][pt.y].kind==LDS_FREE )
                ds->squares[pt.x][pt.y].kind=LDS_WALL;
            else if ( ds->squares[pt.x][pt.y].kind==LDS_WALL )
                ds->squares[pt.x][pt.y].kind=LDS_FREE;
#if ASSERT_ENABLED
            else ASSERT_MESS();
#endif
            break;
#if ASSERT_ENABLED
        default: ASSERT_MESS();
#endif
    }
}

extern void lds_draw_pts(Tlds* ds, TdrawOpt dopt, Tpoints*pts)
{
    int i;
    for (i=0 ; i<pts->nb ; i+=1)
        lds_draw_pt(ds,dopt,pts->t[i]);
}

/*======================================================================*/
/*= check                                                              =*/
/*=                                                                    =*/
/*= These function return 0 if the check is true otherwise 1.          =*/

extern int lds_check_xy      (const Tlds*ds, int x, int y)
{ Tpoint pt; pt.x=x; pt.y=y; return lds_check_pt(ds,pt); }
extern int lds_check_pt      (const Tlds*ds, Tpoint pt)
{ return ! (0<=pt.x && pt.x<ds->dx && 0<=pt.y && pt.y<ds->dy); }

extern int lds_checkborder_pt(const Tlds*ds, Tpoint pt)
{
    if ( lds_check_pt(ds,pt)!=0 ) return 1;
    return ! (pt.x==0 || pt.y==0 || pt.x==(ds->dx-1) || pt.y==(ds->dy-1));
}

extern int lds_check_pts     (const Tlds*ds, Tpoints*pts, Tpoint*err)
{
    int i;
    for (i=0 ; i<pts->nb ; i+=1) {
        if ( lds_check_pt(ds,pts->t[i]) ) {
            *err = pts->t[i];
            return 1;
        }
    }
    return 0;
}

/*======================================================================*/
/*= magic door                                                         =*/

extern Tsqmd* lds_sqmd_new(Tpoint org)
{
    Twr wrd;
    Tsqmd* md = u_malloc0( sizeof(*md) );
    for (wrd=LG_WrFirst ; wrd<LG_WrNb ; wrd+=1) {
        TsqmdEle* mde = md->t+wrd;
      //mde->chg  = 0;
        mde->wrd  = wrd;
        mde->dest = wr_next(wrd,org) ;
    }
    return md;
}

extern Tsqmd* lds_sqmd_new_sqmd(const Tsqmd* md)
{
    Tsqmd* ret = u_malloc( sizeof(*md) );
    *ret = *md;
    return ret;
}

extern void   lds_sqmd_free    (Tsqmd* md)
{ if (md) free ( md ); }

// set the wrd entry of md to des and set chg to 1.
extern Tsqmd* lds_sqmd_update(Tsqmd*md, Twr wrd, Tpoint des)
{
#if 1
    if ( !(0<=wrd && wrd<LG_WrNb) ) ASSERT_MESS();
#endif
    md->t[wrd].chg  = 1;
    md->t[wrd].dest = des;
}

/*======================================================================*/
/*= others                                                             =*/

// bdTs bdTm bdTm bdTx  bdTm bdTm bdTx  bdTm bdTm bdTe
static char bdTs = '+';
static char bdTm = '-';
static char bdTx = '+';
static char bdTe = '+';

static void lds_dump_TOP(Tlds* ds, FILE* ostream,int hlen)
{
    char l_bdTs = ' '; // bdTs
    char l_bdTx = ' '; // bdTx
    char l_bdTe = ' '; // bdTe
    int x,i;
    char buf[10];
    fprintf(ostream,"     %c",l_bdTs);
    for (x=0 ; x<ds->dx ; x+=1) {
        if (x!=0)
            fwrite(&l_bdTx,1,1,ostream);
        if ( hlen>3 ) {
            int len,wc,j;
            len = sprintf(buf,"%d",x);
            for (j=0 ; j<(hlen-len-1) ; j+=1)
                fprintf(ostream," ");
            fprintf(ostream,"%s ",buf);
        } else {
            sprintf(buf,"%10d",x);
            fprintf(ostream,"%s",buf+10-hlen);
        }
    }
    fprintf(ostream,"%c\n",l_bdTe);
}

static void lds_dump_TMB(Tlds* ds, FILE* ostream,int hlen)
{
    int x,i;
    fprintf(ostream,"     %c",bdTs);
    for (x=0 ; x<ds->dx ; x+=1) {
        if (x!=0)
            fwrite(&bdTx,1,1,ostream);
        for (i=0; i<hlen ; i++) 
            fwrite(&bdTm,1,1,ostream);
    }
    fprintf(ostream,"%c\n",bdTe);
}

typedef struct {
    char* m[LDS_SIZE][LDS_SIZE];
} Tm;

static void lds_dump_delM(Tlds* ds, Tm* m)
{
    int x,y;
    for (x=0 ; x<ds->dx ; x+=1) {
        for (y=0 ; y<ds->dy ; y+=1) {
            if (m->m[x][y]!=0) free( m->m[x][y] );
        }
    }
    free( m );
}

static Tm* lds_dump_newM(Tlds* ds)
{
    char  buf[10];
    int x,y,id,whcnt,mdcnt;
    Twr wrd;
    Tsqmd* mdp;
    Tpoint des;
    char**  ps;

    Tm* m = u_malloc0(sizeof(Tm));

    // firstly set the kind of squares
    for (x=0 ; x<ds->dx ; x+=1) {
      for (y=0 ; y<ds->dy ; y+=1) {
        Tsquare*s=&ds->squares[x][y];
        char    tk0;
        switch (s->kind) {
          case LDS_IN:   tk0='E'; break;
          case LDS_OUT:  tk0='O'; break;
          case LDS_WALL: tk0='*'; break;
          case LDS_FREE: tk0=' '; break;
#if ASSERT_ENABLED
          default:
            ASSERT_MESS(); exit(1);
#endif
        }
        m->m[x][y]    = u_malloc0(2);
        m->m[x][y][0] = tk0;
      }
    }

    // second append the option of squares
    whcnt=0; mdcnt=0;
    for (x=0 ; x<ds->dx ; x+=1) {
      for (y=0 ; y<ds->dy ; y+=1) {
        Tsquare*s=&ds->squares[x][y];
        switch ( s->opt ) {
          case LDS_OptWH:
            id = '0'+(whcnt++%10);
            sprintf(buf,"W%c",id);
            break;
          case LDS_OptMD:
            id = 'A'+(mdcnt++%10);
            sprintf(buf,"%c",id);
            break;
          default:       continue;
        }
        // append buf to m[x,y]
        ps = &m->m[x][y];
        *ps = realloc(*ps,strlen(*ps)+strlen(buf)+1);
        strcat(*ps,buf);
      }
    }

    // third append the endpoint information if any.
    whcnt=0; mdcnt=0;
    for (x=0 ; x<ds->dx ; x+=1) {
      for (y=0 ; y<ds->dy ; y+=1) {
        Tsquare*s=&ds->squares[x][y];
        switch ( s->opt ) {
          case LDS_OptWH:
            id = '0'+(whcnt++%10);
            sprintf(buf,"w%c",id);
            des = s->sq_whd;
            // append buf to m[des]
            ps = &m->m[des.x][des.y];
            *ps = realloc(*ps,strlen(*ps)+strlen(buf)+1);
            strcat(*ps,buf);
            break;
          case LDS_OptMD:
            id = 'a'+(mdcnt++%10);
            mdp = s->sq_mdp;
            for (wrd=LG_WrFirst ; wrd<LG_WrNb ; wrd++) {
                TsqmdEle* mde = mdp->t+wrd;
                if (mde->chg==0) continue;
                sprintf(buf,"%c%d",id,wrd%8);
                des = mde->dest;
                // append buf to m[des]
                ps  = &m->m[des.x][des.y];
                *ps = realloc(*ps,strlen(*ps)+strlen(buf)+1);
                strcat(*ps,buf);
            }
            break;
          default:       continue;
        }
      }
    }
    return m;
}

extern void lds_dump(Tlds* ds, FILE* ostream)
{
    int   x,y,i,wc;
    char  space=' ';
    char  bdS='|';
    char  bdM='|';
    char  bdE='|';
    int   hlen = 4;   // the horizontal len of squares (in char).
    Tm*   m = lds_dump_newM(ds);
    fprintf(ostream,"\n");

    lds_dump_TOP(ds,ostream,hlen);
    lds_dump_TMB(ds,ostream,hlen);

    for (y=0 ; y<ds->dy ; y+=1) {
        fprintf(ostream," %4d",y);
        for (x=0 ; x<ds->dx ; x+=1) {
            if ( x==0 )
                fwrite(&bdS,1,1,ostream);
            else
                fwrite(&bdM,1,1,ostream);
            char* str = m->m[x][y];
            int   len = strlen(str);
            if ( str[0]==' ' && len>2 ) {
                str += 1;
                len -= 1;
            }

            for (wc=0; wc<((hlen-len)/2) ; wc++)
                fwrite(&space,1,1,ostream);
            for (i=0 ; i<len && wc<hlen ; i++) {
                fwrite(str+i,1,1,ostream);
                wc+=1;
            }
            for (i=wc; i<hlen ; i++)
                fwrite(&space,1,1,ostream);
        }
        fprintf(ostream,"%c",bdE);
        for (x=0 ; x<ds->dx ; x+=1) {
            char* str = m->m[x][y];
            if ( *str==' ') str += 1;
            int   len = strlen(str);
            if ( len<=hlen ) continue;
            fprintf(ostream," %d:%s",x,m->m[x][y]);
        }
        fprintf(ostream,"\n");
        lds_dump_TMB(ds,ostream,hlen);
    }
    lds_dump_TOP(ds,ostream,hlen);
    fprintf(ostream,"\n");
    lds_dump_delM( ds, m );
}

/*======================================================================*/
