/* glpapi09.c (mixed integer programming routines) */

/***********************************************************************
*  This code is part of GLPK (GNU Linear Programming Kit).
*
*  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013 Andrew Makhorin, Department for Applied
*  Informatics, Moscow Aviation Institute, Moscow, Russia. All rights
*  reserved. E-mail: <mao@gnu.org>.
*
*  GLPK is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  GLPK is distributed in the hope that it will be useful, but WITHOUT
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
*  License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with GLPK. If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "draft.h"
#include "glpenv.h"
#include "glpios.h"
#include "glpnpp.h"

/***********************************************************************
*  NAME
*
*  glp_set_col_kind - set (change) column kind
*
*  SYNOPSIS
*
*  void glp_set_col_kind(glp_prob *mip, int j, int kind);
*
*  DESCRIPTION
*
*  The routine glp_set_col_kind sets (changes) the kind of j-th column
*  (structural variable) as specified by the parameter kind:
*
*  GLP_CV - continuous variable;
*  GLP_IV - integer variable;
*  GLP_BV - binary variable. */

void glp_set_col_kind(glp_prob *mip, int j, int kind)
{     GLPCOL *col;
      if (!(1 <= j && j <= mip->n))
         xerror("glp_set_col_kind: j = %d; column number out of range\n"
            , j);
      col = mip->col[j];
      switch (kind)
      {  case GLP_CV:
            col->kind = GLP_CV;
            break;
         case GLP_IV:
            col->kind = GLP_IV;
            break;
         case GLP_BV:
            col->kind = GLP_IV;
            if (!(col->type == GLP_DB && col->lb == 0.0 && col->ub ==
               1.0)) glp_set_col_bnds(mip, j, GLP_DB, 0.0, 1.0);
            break;
         default:
            xerror("glp_set_col_kind: j = %d; kind = %d; invalid column"
               " kind\n", j, kind);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_get_col_kind - retrieve column kind
*
*  SYNOPSIS
*
*  int glp_get_col_kind(glp_prob *mip, int j);
*
*  RETURNS
*
*  The routine glp_get_col_kind returns the kind of j-th column, i.e.
*  the kind of corresponding structural variable, as follows:
*
*  GLP_CV - continuous variable;
*  GLP_IV - integer variable;
*  GLP_BV - binary variable */

int glp_get_col_kind(glp_prob *mip, int j)
{     GLPCOL *col;
      int kind;
      if (!(1 <= j && j <= mip->n))
         xerror("glp_get_col_kind: j = %d; column number out of range\n"
            , j);
      col = mip->col[j];
      kind = col->kind;
      switch (kind)
      {  case GLP_CV:
            break;
         case GLP_IV:
            if (col->type == GLP_DB && col->lb == 0.0 && col->ub == 1.0)
               kind = GLP_BV;
            break;
         default:
            xassert(kind != kind);
      }
      return kind;
}

/***********************************************************************
*  NAME
*
*  glp_get_num_int - retrieve number of integer columns
*
*  SYNOPSIS
*
*  int glp_get_num_int(glp_prob *mip);
*
*  RETURNS
*
*  The routine glp_get_num_int returns the current number of columns,
*  which are marked as integer. */

int glp_get_num_int(glp_prob *mip)
{     GLPCOL *col;
      int j, count = 0;
      for (j = 1; j <= mip->n; j++)
      {  col = mip->col[j];
         if (col->kind == GLP_IV) count++;
      }
      return count;
}

/***********************************************************************
*  NAME
*
*  glp_get_num_bin - retrieve number of binary columns
*
*  SYNOPSIS
*
*  int glp_get_num_bin(glp_prob *mip);
*
*  RETURNS
*
*  The routine glp_get_num_bin returns the current number of columns,
*  which are marked as binary. */

int glp_get_num_bin(glp_prob *mip)
{     GLPCOL *col;
      int j, count = 0;
      for (j = 1; j <= mip->n; j++)
      {  col = mip->col[j];
         if (col->kind == GLP_IV && col->type == GLP_DB && col->lb ==
            0.0 && col->ub == 1.0) count++;
      }
      return count;
}

/***********************************************************************
*  NAME
*
*  glp_intopt - solve MIP problem with the branch-and-bound method
*
*  SYNOPSIS
*
*  int glp_intopt(glp_prob *P, const glp_iocp *parm);
*
*  DESCRIPTION
*
*  The routine glp_intopt is a driver to the MIP solver based on the
*  branch-and-bound method.
*
*  On entry the problem object should contain optimal solution to LP
*  relaxation (which can be obtained with the routine glp_simplex).
*
*  The MIP solver has a set of control parameters. Values of the control
*  parameters can be passed in a structure glp_iocp, which the parameter
*  parm points to.
*
*  The parameter parm can be specified as NULL, in which case the MIP
*  solver uses default settings.
*
*  RETURNS
*
*  0  The MIP problem instance has been successfully solved. This code
*     does not necessarily mean that the solver has found optimal
*     solution. It only means that the solution process was successful.
*
*  GLP_EBOUND
*     Unable to start the search, because some double-bounded variables
*     have incorrect bounds or some integer variables have non-integer
*     (fractional) bounds.
*
*  GLP_EROOT
*     Unable to start the search, because optimal basis for initial LP
*     relaxation is not provided.
*
*  GLP_EFAIL
*     The search was prematurely terminated due to the solver failure.
*
*  GLP_EMIPGAP
*     The search was prematurely terminated, because the relative mip
*     gap tolerance has been reached.
*
*  GLP_ETMLIM
*     The search was prematurely terminated, because the time limit has
*     been exceeded.
*
*  GLP_ENOPFS
*     The MIP problem instance has no primal feasible solution (only if
*     the MIP presolver is used).
*
*  GLP_ENODFS
*     LP relaxation of the MIP problem instance has no dual feasible
*     solution (only if the MIP presolver is used).
*
*  GLP_ESTOP
*     The search was prematurely terminated by application. */

void glp_init_mip_ctx(glp_mip_ctx *ctx){
    ctx->parm = NULL;
    ctx->tree = NULL;
    ctx->ret = 0;
    ctx->done = 0;
 
    ctx->presolve.npp  = NULL;
    ctx->presolve.mip = NULL;
    ctx->presolve.state = PRE_NONE;
}

static void solve_mip_start(glp_prob *P, glp_mip_ctx *ctx,
      glp_prob *P0 /* problem passed to glp_intopt */,
      NPP *npp /* preprocessor workspace or NULL */)
{     /* solve MIP directly without using the preprocessor */
      const glp_iocp *parm = ctx->parm;
      glp_tree *T;
      /* optimal basis to LP relaxation must be provided */
      if (glp_get_status(P) != GLP_OPT)
      {  if (parm->msg_lev >= GLP_MSG_ERR)
            xprintf("glp_intopt: optimal basis to initial LP relaxation"
               " not provided\n");
         ctx->ret = GLP_EROOT;
         goto done;
      }
      /* it seems all is ok */
      if (parm->msg_lev >= GLP_MSG_ALL)
         xprintf("Integer optimization begins...\n");
      /* create the branch-and-bound tree */
      T = ios_create_tree(P, parm);
#if 1 /* 11/VII-2013 */
      T->P = P0;
      T->npp = npp;
#endif
      ctx->tree = T;
    
      /* solve the problem instance */
      ios_driver_run(T, &ctx->ios);
      ctx->done = ctx->ios.done;
      if (ctx->done) ctx->ret = ctx->ios.ret;
      return;
done:
    ctx->done = 1;
}

static void solve_mip_stop(glp_prob *P, glp_mip_ctx *ctx,
    glp_prob *P0 /* problem passed to glp_intopt */,
    NPP *npp /* preprocessor workspace or NULL */)
{
      int ret = ctx->ret;
      const glp_iocp *parm = ctx->parm;
      glp_tree *T = ctx->tree;
      if (!T) return;

      /* delete the branch-and-bound tree */
      ios_delete_tree(T);
      /* analyze exit code reported by the mip driver */
      if (ret == 0)
      {  if (P->mip_stat == GLP_FEAS)
         {  if (parm->msg_lev >= GLP_MSG_ALL)
               xprintf("INTEGER OPTIMAL SOLUTION FOUND\n");
            P->mip_stat = GLP_OPT;
         }
         else
         {  if (parm->msg_lev >= GLP_MSG_ALL)
               xprintf("PROBLEM HAS NO INTEGER FEASIBLE SOLUTION\n");
            P->mip_stat = GLP_NOFEAS;
         }
      }
      else if (ret == GLP_EMIPGAP)
      {  if (parm->msg_lev >= GLP_MSG_ALL)
            xprintf("RELATIVE MIP GAP TOLERANCE REACHED; SEARCH TERMINA"
               "TED\n");
      }
      else if (ret == GLP_ETMLIM)
      {  if (parm->msg_lev >= GLP_MSG_ALL)
            xprintf("TIME LIMIT EXCEEDED; SEARCH TERMINATED\n");
      }
      else if (ret == GLP_EFAIL)
      {  if (parm->msg_lev >= GLP_MSG_ERR)
            xprintf("glp_intopt: cannot solve current LP relaxation\n");
      }
      else if (ret == GLP_ESTOP)
      {  if (parm->msg_lev >= GLP_MSG_ALL)
            xprintf("SEARCH TERMINATED BY APPLICATION\n");
      }
      else
         xassert(ret != ret);
}

static void preprocess_and_solve_mip_start(glp_prob *P, glp_mip_ctx *ctx)
{     
      /* solve MIP using the preprocessor */
      const glp_iocp *parm = ctx->parm;
#ifdef HAVE_ENV
      ENV *env = get_env_ptr();
      int term_out = env->term_out;
#endif
      NPP *npp;
      glp_prob *mip = NULL;
      glp_bfcp bfcp;
      glp_smcp smcp;
      
	  ctx->presolve.state = PRE_CLEAN;

	  if (parm->msg_lev >= GLP_MSG_ALL)
         xprintf("Preprocessing...\n");
      /* create preprocessor workspace */
      ctx->presolve.npp = npp = npp_create_wksp();
      /* load original problem into the preprocessor workspace */
      npp_load_prob(npp, P, GLP_OFF, GLP_MIP, GLP_OFF);
      /* process MIP prior to applying the branch-and-bound method */
#ifdef HAVE_ENV
      if (!term_out || parm->msg_lev < GLP_MSG_ALL)
         env->term_out = GLP_OFF;
      else
         env->term_out = GLP_ON;
      ctx->ret = npp_integer(npp, parm);
      env->term_out = term_out;
#else
    ctx->ret = npp_integer(npp, parm);
#endif
      if (ctx->ret == 0)
         ;
      else if (ctx->ret == GLP_ENOPFS)
      {  if (parm->msg_lev >= GLP_MSG_ALL)
            xprintf("PROBLEM HAS NO PRIMAL FEASIBLE SOLUTION\n");
      }
      else if (ctx->ret == GLP_ENODFS)
      {  if (parm->msg_lev >= GLP_MSG_ALL)
            xprintf("LP RELAXATION HAS NO DUAL FEASIBLE SOLUTION\n");
      }
      else
         xassert(ctx->ret != ctx->ret);
      if (ctx->ret != 0) {
          ctx->presolve.state = PRE_DONE;
          goto done;
      };
      /* build transformed MIP */
      mip = glp_create_prob();
      ctx->presolve.mip = mip;
      npp_build_prob(npp, mip);
      /* if the transformed MIP is empty, it has empty solution, which
         is optimal */
      if (mip->m == 0 && mip->n == 0)
      {  mip->mip_stat = GLP_OPT;
         mip->mip_obj = mip->c0;
         if (parm->msg_lev >= GLP_MSG_ALL)
         {  xprintf("Objective value = %17.9e\n", mip->mip_obj);
            xprintf("INTEGER OPTIMAL SOLUTION FOUND BY MIP PREPROCESSOR"
               "\n");
         }
         
         ctx->presolve.state = PRE_POST;
         goto done;
      }
      /* display some statistics */
      if (parm->msg_lev >= GLP_MSG_ALL)
      {  int ni = glp_get_num_int(mip);
         int nb = glp_get_num_bin(mip);
         char s[50];
         xprintf("%d row%s, %d column%s, %d non-zero%s\n",
            mip->m, mip->m == 1 ? "" : "s", mip->n, mip->n == 1 ? "" :
            "s", mip->nnz, mip->nnz == 1 ? "" : "s");
         if (nb == 0)
            strcpy(s, "none of");
         else if (ni == 1 && nb == 1)
            strcpy(s, "");
         else if (nb == 1)
            strcpy(s, "one of");
         else if (nb == ni)
            strcpy(s, "all of");
         else
            sprintf(s, "%d of", nb);
         xprintf("%d integer variable%s, %s which %s binary\n",
            ni, ni == 1 ? "" : "s", s, nb == 1 ? "is" : "are");
      }
      /* inherit basis factorization control parameters */
      glp_get_bfcp(P, &bfcp);
      glp_set_bfcp(mip, &bfcp);
      /* scale the transformed problem */
#ifdef HAVE_ENV
      if (!term_out || parm->msg_lev < GLP_MSG_ALL)
         env->term_out = GLP_OFF;
      else
         env->term_out = GLP_ON;
      glp_scale_prob(mip,
         GLP_SF_GM | GLP_SF_EQ | GLP_SF_2N | GLP_SF_SKIP);
      env->term_out = term_out;
      /* build advanced initial basis */
      if (!term_out || parm->msg_lev < GLP_MSG_ALL)
         env->term_out = GLP_OFF;
      else
         env->term_out = GLP_ON;
      glp_adv_basis(mip, 0);
      env->term_out = term_out;
#else
      glp_scale_prob(mip,
        GLP_SF_GM | GLP_SF_EQ | GLP_SF_2N | GLP_SF_SKIP);
      glp_adv_basis(mip, 0);
#endif
      /* solve initial LP relaxation */
      if (parm->msg_lev >= GLP_MSG_ALL)
         xprintf("Solving LP relaxation...\n");
      glp_init_smcp(&smcp);
      smcp.msg_lev = parm->msg_lev;
      mip->it_cnt = P->it_cnt;
      ctx->ret = glp_simplex(mip, &smcp);
      P->it_cnt = mip->it_cnt;
      if (ctx->ret != 0)
      {  if (parm->msg_lev >= GLP_MSG_ERR)
            xprintf("glp_intopt: cannot solve LP relaxation\n");
         ctx->ret = GLP_EFAIL;
         ctx->presolve.state = PRE_DONE;
         goto done;
      }
      /* check status of the basic solution */
      ctx->ret = glp_get_status(mip);
      if (ctx->ret == GLP_OPT)
         ctx->ret = 0;
      else if (ctx->ret == GLP_NOFEAS)
         ctx->ret = GLP_ENOPFS;
      else if (ctx->ret == GLP_UNBND)
         ctx->ret = GLP_ENODFS;
      else
         xassert(ctx->ret != ctx->ret);
      if (ctx->ret != 0){
        ctx->presolve.state = PRE_DONE; // done
        goto done;
      };
      /* solve the transformed MIP */
      mip->it_cnt = P->it_cnt;

      if (parm->use_sol)
      {  mip->mip_stat = P->mip_stat;
         mip->mip_obj = P->mip_obj;
      }
    
      solve_mip_start(mip, ctx, P, npp);
      return;
done:
    ctx->done = 1;
}

static void preprocess_and_solve_mip_stop(glp_prob *P, glp_mip_ctx *ctx)
{
    glp_mip_ctx_presolve_state state = ctx->presolve.state;
    if (state == PRE_NONE) return;
    if (state == PRE_CLEAN) goto start;
    if (state == PRE_POST) goto post;
    if (state == PRE_DONE) goto done;
    xassert(state != state);

start:
      P->it_cnt = ctx->presolve.mip->it_cnt;
      /* only integer feasible solution can be postprocessed */
      if (!(ctx->presolve.mip->mip_stat == GLP_OPT || ctx->presolve.mip->mip_stat == GLP_FEAS))
      {  P->mip_stat = ctx->presolve.mip->mip_stat;
         goto done;
      }
      /* postprocess solution from the transformed MIP */
post: npp_postprocess(ctx->presolve.npp, ctx->presolve.mip);
      /* the transformed MIP is no longer needed */
      glp_delete_prob(ctx->presolve.mip), ctx->presolve.mip = NULL;
      /* store solution to the original problem */
      npp_unload_sol(ctx->presolve.npp, P);
done: /* delete the transformed MIP, if it exists */
      if (ctx->presolve.mip != NULL) glp_delete_prob(ctx->presolve.mip);
      /* delete preprocessor workspace */
      npp_delete_wksp(ctx->presolve.npp);
}

#ifndef HAVE_ALIEN_SOLVER /* 28/V-2010 */
int _glp_intopt1(glp_prob *P, const glp_iocp *parm)
{     xassert(P == P);
      xassert(parm == parm);
      xprintf("glp_intopt: no alien solver is available\n");
      return GLP_EFAIL;
}
#endif

void glp_intopt_start(glp_prob *P, glp_mip_ctx *ctx)
{     /* solve MIP problem with the branch-and-bound method */
      int i, j;
      const glp_iocp *parm = ctx->parm;
      /* check problem object */
      if (P == NULL || P->magic != GLP_PROB_MAGIC)
         xerror("glp_intopt: P = %p; invalid problem object\n", P);
      if (P->tree != NULL)
         xerror("glp_intopt: operation not allowed\n");
      if (!(parm->msg_lev == GLP_MSG_OFF ||
            parm->msg_lev == GLP_MSG_ERR ||
            parm->msg_lev == GLP_MSG_ON  ||
            parm->msg_lev == GLP_MSG_ALL ||
            parm->msg_lev == GLP_MSG_DBG))
         xerror("glp_intopt: msg_lev = %d; invalid parameter\n",
            parm->msg_lev);
      if (!(parm->br_tech == GLP_BR_FFV ||
            parm->br_tech == GLP_BR_LFV ||
            parm->br_tech == GLP_BR_MFV ||
            parm->br_tech == GLP_BR_DTH ||
            parm->br_tech == GLP_BR_PCH))
         xerror("glp_intopt: br_tech = %d; invalid parameter\n",
            parm->br_tech);
      if (!(parm->bt_tech == GLP_BT_DFS ||
            parm->bt_tech == GLP_BT_BFS ||
            parm->bt_tech == GLP_BT_BLB ||
            parm->bt_tech == GLP_BT_BPH))
         xerror("glp_intopt: bt_tech = %d; invalid parameter\n",
            parm->bt_tech);
      if (!(0.0 < parm->tol_int && parm->tol_int < 1.0))
         xerror("glp_intopt: tol_int = %g; invalid parameter\n",
            parm->tol_int);
      if (!(0.0 < parm->tol_obj && parm->tol_obj < 1.0))
         xerror("glp_intopt: tol_obj = %g; invalid parameter\n",
            parm->tol_obj);
      if (parm->tm_lim < 0)
         xerror("glp_intopt: tm_lim = %d; invalid parameter\n",
            parm->tm_lim);
      if (parm->out_frq < 0)
         xerror("glp_intopt: out_frq = %d; invalid parameter\n",
            parm->out_frq);
      if (parm->out_dly < 0)
         xerror("glp_intopt: out_dly = %d; invalid parameter\n",
            parm->out_dly);
      if (!(0 <= parm->cb_size && parm->cb_size <= 256))
         xerror("glp_intopt: cb_size = %d; invalid parameter\n",
            parm->cb_size);
      if (!(parm->pp_tech == GLP_PP_NONE ||
            parm->pp_tech == GLP_PP_ROOT ||
            parm->pp_tech == GLP_PP_ALL))
         xerror("glp_intopt: pp_tech = %d; invalid parameter\n",
            parm->pp_tech);
      if (parm->mip_gap < 0.0)
         xerror("glp_intopt: mip_gap = %g; invalid parameter\n",
            parm->mip_gap);
      if (!(parm->mir_cuts == GLP_ON || parm->mir_cuts == GLP_OFF))
         xerror("glp_intopt: mir_cuts = %d; invalid parameter\n",
            parm->mir_cuts);
      if (!(parm->gmi_cuts == GLP_ON || parm->gmi_cuts == GLP_OFF))
         xerror("glp_intopt: gmi_cuts = %d; invalid parameter\n",
            parm->gmi_cuts);
      if (!(parm->cov_cuts == GLP_ON || parm->cov_cuts == GLP_OFF))
         xerror("glp_intopt: cov_cuts = %d; invalid parameter\n",
            parm->cov_cuts);
      if (!(parm->clq_cuts == GLP_ON || parm->clq_cuts == GLP_OFF))
         xerror("glp_intopt: clq_cuts = %d; invalid parameter\n",
            parm->clq_cuts);
      if (!(parm->presolve == GLP_ON || parm->presolve == GLP_OFF))
         xerror("glp_intopt: presolve = %d; invalid parameter\n",
            parm->presolve);
      if (!(parm->binarize == GLP_ON || parm->binarize == GLP_OFF))
         xerror("glp_intopt: binarize = %d; invalid parameter\n",
            parm->binarize);
      if (!(parm->fp_heur == GLP_ON || parm->fp_heur == GLP_OFF))
         xerror("glp_intopt: fp_heur = %d; invalid parameter\n",
            parm->fp_heur);
#if 1 /* 28/V-2010 */
      if (!(parm->alien == GLP_ON || parm->alien == GLP_OFF))
         xerror("glp_intopt: alien = %d; invalid parameter\n",
            parm->alien);
#endif
#if 0 /* 11/VII-2013 */
      /* integer solution is currently undefined */
      P->mip_stat = GLP_UNDEF;
      P->mip_obj = 0.0;
#else
      if (!parm->use_sol)
         P->mip_stat = GLP_UNDEF;
      if (P->mip_stat == GLP_NOFEAS)
         P->mip_stat = GLP_UNDEF;
      if (P->mip_stat == GLP_UNDEF)
         P->mip_obj = 0.0;
      else if (P->mip_stat == GLP_OPT)
         P->mip_stat = GLP_FEAS;
#endif
      /* check bounds of double-bounded variables */
      for (i = 1; i <= P->m; i++)
      {  GLPROW *row = P->row[i];
         if (row->type == GLP_DB && row->lb >= row->ub)
         {  if (parm->msg_lev >= GLP_MSG_ERR)
               xprintf("glp_intopt: row %d: lb = %g, ub = %g; incorrect"
                  " bounds\n", i, row->lb, row->ub);
            ctx->ret = GLP_EBOUND;
            return;
         }
      }
      for (j = 1; j <= P->n; j++)
      {  GLPCOL *col = P->col[j];
         if (col->type == GLP_DB && col->lb >= col->ub)
         {  if (parm->msg_lev >= GLP_MSG_ERR)
               xprintf("glp_intopt: column %d: lb = %g, ub = %g; incorr"
                  "ect bounds\n", j, col->lb, col->ub);
            ctx->ret = GLP_EBOUND;
            return;
         }
      }
      /* bounds of all integer variables must be integral */
      for (j = 1; j <= P->n; j++)
      {  GLPCOL *col = P->col[j];
         if (col->kind != GLP_IV) continue;
         if (col->type == GLP_LO || col->type == GLP_DB)
         {  if (col->lb != floor(col->lb))
            {  if (parm->msg_lev >= GLP_MSG_ERR)
                  xprintf("glp_intopt: integer column %d has non-intege"
                     "r lower bound %g\n", j, col->lb);
               ctx->ret = GLP_EBOUND;
               return;
            }
         }
         if (col->type == GLP_UP || col->type == GLP_DB)
         {  if (col->ub != floor(col->ub))
            {  if (parm->msg_lev >= GLP_MSG_ERR)
                  xprintf("glp_intopt: integer column %d has non-intege"
                     "r upper bound %g\n", j, col->ub);
               ctx->ret = GLP_EBOUND;
               return;
            }
         }
         if (col->type == GLP_FX)
         {  if (col->lb != floor(col->lb))
            {  if (parm->msg_lev >= GLP_MSG_ERR)
                  xprintf("glp_intopt: integer column %d has non-intege"
                     "r fixed value %g\n", j, col->lb);
               ctx->ret = GLP_EBOUND;
               return;
            }
         }
      }
      /* solve MIP problem */
      if (parm->msg_lev >= GLP_MSG_ALL)
      {  int ni = glp_get_num_int(P);
         int nb = glp_get_num_bin(P);
         char s[50];
         xprintf("GLPK Integer Optimizer, v%d.%d\n", GLP_MAJOR_VERSION, GLP_MINOR_VERSION);
         xprintf("%d row%s, %d column%s, %d non-zero%s\n",
            P->m, P->m == 1 ? "" : "s", P->n, P->n == 1 ? "" : "s",
            P->nnz, P->nnz == 1 ? "" : "s");
         if (nb == 0)
            strcpy(s, "none of");
         else if (ni == 1 && nb == 1)
            strcpy(s, "");
         else if (nb == 1)
            strcpy(s, "one of");
         else if (nb == ni)
            strcpy(s, "all of");
         else
            sprintf(s, "%d of", nb);
         xprintf("%d integer variable%s, %s which %s binary\n",
            ni, ni == 1 ? "" : "s", s, nb == 1 ? "is" : "are");
      }
#if 1 /* 28/V-2010 */
      if (parm->alien)
      {  /* use alien integer optimizer */
         ctx->ret = _glp_intopt1(P, parm);
         return;
      }
#endif
    if (!parm->presolve)
        solve_mip_start(P, ctx, P, NULL);
      else
        preprocess_and_solve_mip_start(P, ctx);
}

void glp_intopt_stop(glp_prob *P, glp_mip_ctx *ctx)
{
    preprocess_and_solve_mip_stop(P, ctx);
    solve_mip_stop(P, ctx, P, NULL);
    
#if 1 /* 12/III-2013 */
    if (ctx->ret == GLP_ENOPFS)
        P->mip_stat = GLP_NOFEAS;
#endif
}

void glp_intopt_run(glp_mip_ctx *ctx)
{
    ios_driver_run(ctx->tree, &ctx->ios);
    ctx->done = ctx->ios.done;
    if (ctx->done) ctx->ret = ctx->ios.ret;
}

int glp_intopt(glp_prob *P, const glp_iocp *parm)
{
    glp_iocp _parm;
    glp_mip_ctx ctx;
    
    glp_init_mip_ctx(&ctx);
    /* check control parameters */
    if (parm == NULL)
        parm = &_parm, glp_init_iocp((glp_iocp *)parm);
    ctx.parm = parm;
    
    glp_intopt_start(P, &ctx);
    while (!ctx.done){
        glp_tree *T = ctx.tree;
        xassert(T->parm->cb_func);
        T->parm->cb_func(T, T->parm->cb_info);
        
        glp_intopt_run(&ctx);
    }
    glp_intopt_stop(P, &ctx);
    return ctx.ret;
}

/***********************************************************************
*  NAME
*
*  glp_init_iocp - initialize integer optimizer control parameters
*
*  SYNOPSIS
*
*  void glp_init_iocp(glp_iocp *parm);
*
*  DESCRIPTION
*
*  The routine glp_init_iocp initializes control parameters, which are
*  used by the integer optimizer, with default values.
*
*  Default values of the control parameters are stored in a glp_iocp
*  structure, which the parameter parm points to. */

void glp_init_iocp(glp_iocp *parm)
{     parm->msg_lev = GLP_MSG_ALL;
      parm->br_tech = GLP_BR_DTH;
      parm->bt_tech = GLP_BT_BLB;
      parm->tol_int = 1e-5;
      parm->tol_obj = 1e-7;
      parm->tm_lim = INT_MAX;
      parm->out_frq = 5000;
      parm->out_dly = 10000;
      parm->cb_func = NULL;
      parm->cb_info = NULL;
      parm->cb_reasons = 0xff;
      parm->cb_size = 0;
      parm->pp_tech = GLP_PP_ALL;
      parm->mip_gap = 0.0;
      parm->mir_cuts = GLP_OFF;
      parm->gmi_cuts = GLP_OFF;
      parm->cov_cuts = GLP_OFF;
      parm->clq_cuts = GLP_OFF;
      parm->presolve = GLP_OFF;
      parm->binarize = GLP_OFF;
      parm->fp_heur = GLP_OFF;
      parm->ps_heur = GLP_OFF;
      parm->ps_tm_lim = 60000; /* 1 minute */
      parm->sr_heur = GLP_ON;
#if 1 /* 24/X-2015; not documented--should not be used */
      parm->use_sol = GLP_OFF;
      parm->save_sol = NULL;
      parm->alien = GLP_OFF;
#endif
      return;
}

/***********************************************************************
*  NAME
*
*  glp_mip_status - retrieve status of MIP solution
*
*  SYNOPSIS
*
*  int glp_mip_status(glp_prob *mip);
*
*  RETURNS
*
*  The routine lpx_mip_status reports the status of MIP solution found
*  by the branch-and-bound solver as follows:
*
*  GLP_UNDEF  - MIP solution is undefined;
*  GLP_OPT    - MIP solution is integer optimal;
*  GLP_FEAS   - MIP solution is integer feasible but its optimality
*               (or non-optimality) has not been proven, perhaps due to
*               premature termination of the search;
*  GLP_NOFEAS - problem has no integer feasible solution (proven by the
*               solver). */

int glp_mip_status(glp_prob *mip)
{     int mip_stat = mip->mip_stat;
      return mip_stat;
}

/***********************************************************************
*  NAME
*
*  glp_mip_obj_val - retrieve objective value (MIP solution)
*
*  SYNOPSIS
*
*  double glp_mip_obj_val(glp_prob *mip);
*
*  RETURNS
*
*  The routine glp_mip_obj_val returns value of the objective function
*  for MIP solution. */

double glp_mip_obj_val(glp_prob *mip)
{     /*struct LPXCPS *cps = mip->cps;*/
      double z;
      z = mip->mip_obj;
      /*if (cps->round && fabs(z) < 1e-9) z = 0.0;*/
      return z;
}

/***********************************************************************
*  NAME
*
*  glp_mip_row_val - retrieve row value (MIP solution)
*
*  SYNOPSIS
*
*  double glp_mip_row_val(glp_prob *mip, int i);
*
*  RETURNS
*
*  The routine glp_mip_row_val returns value of the auxiliary variable
*  associated with i-th row. */

double glp_mip_row_val(glp_prob *mip, int i)
{     /*struct LPXCPS *cps = mip->cps;*/
      double mipx;
      if (!(1 <= i && i <= mip->m))
         xerror("glp_mip_row_val: i = %d; row number out of range\n", i)
            ;
      mipx = mip->row[i]->mipx;
      /*if (cps->round && fabs(mipx) < 1e-9) mipx = 0.0;*/
      return mipx;
}

/***********************************************************************
*  NAME
*
*  glp_mip_col_val - retrieve column value (MIP solution)
*
*  SYNOPSIS
*
*  double glp_mip_col_val(glp_prob *mip, int j);
*
*  RETURNS
*
*  The routine glp_mip_col_val returns value of the structural variable
*  associated with j-th column. */

double glp_mip_col_val(glp_prob *mip, int j)
{     /*struct LPXCPS *cps = mip->cps;*/
      double mipx;
      if (!(1 <= j && j <= mip->n))
         xerror("glp_mip_col_val: j = %d; column number out of range\n",
            j);
      mipx = mip->col[j]->mipx;
      /*if (cps->round && fabs(mipx) < 1e-9) mipx = 0.0;*/
      return mipx;
}

/* eof */
