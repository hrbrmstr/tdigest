#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP is_null_xptr_(SEXP);
extern SEXP Rtd_add(SEXP, SEXP, SEXP);
extern SEXP Rtd_create(SEXP);
extern SEXP Rtd_merge(SEXP, SEXP);
extern SEXP Rtd_quantile_of(SEXP, SEXP);
extern SEXP Rtd_total_count(SEXP);
extern SEXP Rtd_value_at(SEXP, SEXP);
extern SEXP Rtdig(SEXP, SEXP);
extern SEXP Rtquant(SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"is_null_xptr_",   (DL_FUNC) &is_null_xptr_,   1},
    {"Rtd_add",         (DL_FUNC) &Rtd_add,         3},
    {"Rtd_create",      (DL_FUNC) &Rtd_create,      1},
    {"Rtd_merge",       (DL_FUNC) &Rtd_merge,       2},
    {"Rtd_quantile_of", (DL_FUNC) &Rtd_quantile_of, 2},
    {"Rtd_total_count", (DL_FUNC) &Rtd_total_count, 1},
    {"Rtd_value_at",    (DL_FUNC) &Rtd_value_at,    2},
    {"Rtdig",           (DL_FUNC) &Rtdig,           2},
    {"Rtquant",         (DL_FUNC) &Rtquant,         2},
    {NULL, NULL, 0}
};

void R_init_tdigest(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
