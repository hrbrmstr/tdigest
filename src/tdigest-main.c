#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <R_ext/Altrep.h>
#include <R_ext/Itermacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "tdigest.h"

#define O_COMPRESSION 0
#define O_CAP 1
#define O_MNODES 2
#define O_UMNODES 3
#define O_MCOUNT 4
#define O_UMCOUNT 5
#define O_OUT_NODES 6

#define O_MEANS 0
#define O_COUNTS 1

// next 2 ƒ() via <https://github.com/randy3k/xptr/blob/master/src/xptr.c>

void check_is_xptr(SEXP s) {
  if (TYPEOF(s) != EXTPTRSXP) {
    error("expected an externalptr");
  }
}

SEXP is_null_xptr_(SEXP s) {
  check_is_xptr(s);
  return Rf_ScalarLogical(R_ExternalPtrAddr(s) == NULL);
}

static void td_finalizer(SEXP ptr) {
  if(!R_ExternalPtrAddr(ptr)) return;
  td_free((td_histogram_t *)R_ExternalPtrAddr(ptr));
  R_ClearExternalPtr(ptr); /* not really needed */
}

SEXP Rtd_create(SEXP compression) {
  SEXP ptr;
  td_histogram_t *t = td_new(asReal(compression));
  if (t) {
    ptr = PROTECT(R_MakeExternalPtr(t, install("tdigest"), R_NilValue));
    R_RegisterCFinalizerEx(ptr, td_finalizer, TRUE);
    setAttrib(ptr, install("class"), PROTECT(mkString("tdigest")));
    UNPROTECT(2);
    return(ptr);
  } else {
    return(R_NilValue);
  }
}

SEXP Rtdig(SEXP vec, SEXP compression) {
  SEXP ptr;
  td_histogram_t *t = td_new(asReal(compression));
  if (t) {
    R_xlen_t n = Rf_xlength(vec);
    if (ALTREP(vec)) {
      ITERATE_BY_REGION(vec, x, i, nbatch, double, REAL, {
        for (R_xlen_t k = 0; k < nbatch; k++) {
          if (!ISNAN(x[k])) td_add(t, x[k], 1);
        }
      });
    } else {
      for (R_xlen_t i = 0; i < n; i++) {
        if (!ISNAN(REAL(vec)[i])) td_add(t, REAL(vec)[i], 1);
      }
    }
    ptr = PROTECT(R_MakeExternalPtr(t, install("tdigest"), R_NilValue));
    R_RegisterCFinalizerEx(ptr, td_finalizer, TRUE);
    setAttrib(ptr, install("class"), PROTECT(mkString("tdigest")));
    UNPROTECT(2);
    return(ptr);
  } else {
    return(R_NilValue);
  }
}

SEXP Rtquant(SEXP tdig, SEXP probs) {
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(tdig);
  if (t) {
    R_xlen_t n = xlength(probs);
    SEXP out = PROTECT(allocVector(REALSXP, n));
    double *o = REAL(out);
    for (R_xlen_t i = 0; i < n; i++) {
      o[i] = td_value_at(t, REAL(probs)[i]);
    }
    UNPROTECT(1);
    return(out);
  }
  return(R_NilValue);
}

SEXP Rtd_add(SEXP tdig, SEXP val, SEXP count) {
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(tdig);
  if (t) {
    td_add(t, asReal(val), asReal(count)); //void td_add(td_histogram_t *h, double val, double count);
    return(tdig);
  }
  return(R_NilValue);
}

SEXP Rtd_value_at(SEXP tdig, SEXP q) {
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(tdig);
  if (t) {
    return(ScalarReal(td_value_at(t, asReal(q)))); //double td_value_at(td_histogram_t *h, double q);
  }
  return(R_NilValue);
}

SEXP Rtd_quantile_of(SEXP tdig, SEXP val) {
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(tdig);
  if (t) {
    return(ScalarReal(td_quantile_of(t, asReal(val)))); //double td_quantile_of(td_histogram_t *h, double val);
  }
  return(R_NilValue);
}

SEXP Rtd_total_count(SEXP tdig) {
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(tdig);
  if (t) {
    return(ScalarReal(td_total_count(t))); //double td_total_count(td_histogram_t *h);
  }
  return(R_NilValue);
}

SEXP Rtd_merge(SEXP from, SEXP into) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(into);
  if (t) {
    td_merge(t, f);
    return(into); //void td_merge(td_histogram_t *into, td_histogram_t *from);
  }
  return(R_NilValue);
}

SEXP Rg_compression(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarReal(f->compression) : R_NilValue);
}

SEXP Rg_cap(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarInteger(f->cap) : R_NilValue);
}

SEXP Rg_merged_nodes(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarInteger(f->merged_nodes) : R_NilValue);
}

SEXP Rg_unmerged_nodes(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarInteger(f->unmerged_nodes) : R_NilValue);
}

SEXP Rg_merged_count(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarReal(f->merged_count) : R_NilValue);
}

SEXP Rg_unmerged_count(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  return(f ? ScalarReal(f->unmerged_count) : R_NilValue);
}

SEXP Rg_nodes_mean(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  if (f) {
    int N = f->merged_nodes + f->unmerged_nodes;
    SEXP out = PROTECT(allocVector(REALSXP, N));
    for (int i=0; i<N; i++) {
      REAL(out)[i] = f->nodes[i].mean;
    }
    UNPROTECT(1);
    return(out);
  } else {
    return(R_NilValue);
  }
}

SEXP Rg_nodes_count(SEXP from) {
  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);
  if (f) {
    int N = f->merged_nodes + f->unmerged_nodes;
    SEXP out = PROTECT(allocVector(REALSXP, N));
    for (int i=0; i<N; i++) {
      REAL(out)[i] = f->nodes[i].count;
    }
    UNPROTECT(1);
    return(out);
  } else {
    return(R_NilValue);
  }
}

SEXP Rg_toR(SEXP from) {

  td_histogram_t *f = (td_histogram_t *)R_ExternalPtrAddr(from);

  if (f) {

    SEXP o_cap = PROTECT(ScalarInteger(f->cap));
    SEXP o_compression = PROTECT(ScalarReal(f->compression));
    SEXP o_mcount = PROTECT(ScalarReal(f->merged_count));
    SEXP o_umcount = PROTECT(ScalarReal(f->unmerged_count));
    SEXP o_mnodes = PROTECT(ScalarInteger(f->merged_nodes));
    SEXP o_umnodes = PROTECT(ScalarInteger(f->unmerged_nodes));

    int N = f->merged_nodes + f->unmerged_nodes;

    SEXP o_means = PROTECT(allocVector(REALSXP, N));
    SEXP o_counts = PROTECT(allocVector(REALSXP, N));

    for (int i=0; i<N; i++) {
      REAL(o_means)[i] = f->nodes[i].mean;
      REAL(o_counts)[i] = f->nodes[i].count;
    }

    const char *names[] = {
      "compression",
      "cap",
      "merged_nodes",
      "unmerged_nodes",
      "merged_count",
      "unmerged_count",
      "nodes",
      ""
    };

    SEXP out = PROTECT(mkNamed(VECSXP, names));

    SET_VECTOR_ELT(out, O_COMPRESSION, o_compression);
    SET_VECTOR_ELT(out, O_CAP, o_cap);
    SET_VECTOR_ELT(out, O_MNODES, o_mnodes);
    SET_VECTOR_ELT(out, O_UMNODES, o_umnodes);
    SET_VECTOR_ELT(out, O_MCOUNT, o_mcount);
    SET_VECTOR_ELT(out, O_UMCOUNT, o_umcount);

    const char *node_names[] = {
      "counts",
      "means",
      ""
    };

    SEXP out_nodes = PROTECT(mkNamed(VECSXP, node_names));

    SET_VECTOR_ELT(out_nodes, 0, o_means);
    SET_VECTOR_ELT(out_nodes, 1, o_counts);

    SET_VECTOR_ELT(out, O_OUT_NODES, out_nodes);

    UNPROTECT(10);

    return(out);

  } else {
    return(R_NilValue);
  }

}

SEXP Rg_fromR(SEXP td_list) {

  SEXP out = PROTECT(Rtd_create(VECTOR_ELT(td_list, O_COMPRESSION)));

  td_histogram_t *t = (td_histogram_t *)R_ExternalPtrAddr(out);

  t->compression = asReal(VECTOR_ELT(td_list, O_COMPRESSION));
  t->cap = asInteger(VECTOR_ELT(td_list, O_CAP));
  t->merged_nodes = asInteger(VECTOR_ELT(td_list, O_MNODES));
  t->unmerged_nodes = asInteger(VECTOR_ELT(td_list, O_UMNODES));
  t->merged_count = asReal(VECTOR_ELT(td_list, O_MCOUNT));
  t->unmerged_count = asReal(VECTOR_ELT(td_list, O_UMCOUNT));

  int N = t->merged_nodes + t->unmerged_nodes;

  SEXP node_list = VECTOR_ELT(td_list, O_OUT_NODES);

  SEXP o_means = VECTOR_ELT(node_list, O_MEANS);
  SEXP o_counts = VECTOR_ELT(node_list, O_COUNTS);

  for (int i=0; i<N; i++) {
    t->nodes[i].count = REAL(o_counts)[i];
    t->nodes[i].mean = REAL(o_means)[i];
  }

  UNPROTECT(1);

  return(out);

}
