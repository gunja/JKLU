#include "edu_ufl_cise_klu_wrapper_SuperLUMTWrapper.h"
#include <stdio.h>
//#include "slu_ddefs.h"
#include "slu_mt_ddefs.h"

#include <time.h>

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_dCreate_1CompCol_1Matrix
  (JNIEnv *env, jclass cls, jobject superMat, jint rows, jint cols, jint bum,
	jdoubleArray arra, jintArray size, jintArray iArra, jobject ob, jobject ob2, jobject ob3) {
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT jdoubleArray JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_dGenXtrue
  (JNIEnv *env, jclass cls, jint sz, jint sz2, jint sz3)
{
	printf("Called %s\n", __func__ );
	return NULL;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_dFillRHS
  (JNIEnv *env, jclass cls, jobject obj, jint sz, jdoubleArray dArra, jint sz2,
	jobject obj1, jobject obj2)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_get_1perm_1c
  (JNIEnv *env, jclass cls, jint sz, jobject obj1, jintArray iArra)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_dgssv
  (JNIEnv *env, jclass cls, jobjectArray objArra, jobject obj1, jintArray iArra,
	jintArray iArra2, jobject obj2, jobject obj3, jobject obj4,
	jobjectArray objArra2, jintArray iArra3)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_dinf_1norm_1error
  (JNIEnv *env, jclass cls, jint sz, jobject obj1, jdoubleArray dArra)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_ccs_1components_1b_1dgssv
  (JNIEnv *env, jclass cld, jint rows, jint cols, jintArray colsPtrs, jintArray rowPointed,
	jdoubleArray values, jdoubleArray b_vector)
{
return;
#if 0
	SuperMatrix A;
    SuperMatrix L;      /* factor L */
    SCformat *Lstore;
    SuperMatrix U;      /* factor U */
    NCformat *Ustore;
    SuperMatrix B;
    superlu_options_t options;


int      *perm_c; /* column permutation vector */
int      *perm_r; /* row permutations from partial pivoting */
int      nrhs, ldx, info, m, n, nnz;
double   *xact, *rhs;
mem_usage_t   mem_usage;
SuperLUStat_t stat;

struct timespec all_start, solve_start, solve_stop, all_stop;

    jsize vals_size, cols_size, row_size, b_size;

    fprintf(stderr, "Called %s\n", __func__ );
    vals_size = (*env)->GetArrayLength(env, values);
    cols_size = (*env)->GetArrayLength(env, colsPtrs);
    row_size = (*env)->GetArrayLength(env, rowPointed);
    b_size = (*env)->GetArrayLength(env, b_vector);

    nnz = vals_size;
    m = rows;
    n = cols;
    nrhs = 1;

    jdouble *values_double = (*env)->GetDoubleArrayElements(env, values, 0);
    jint *rows_int = (*env)->GetIntArrayElements(env, rowPointed, 0);
    jint *cols_int = (*env)->GetIntArrayElements(env, colsPtrs, 0);
    jdouble *b_double = (*env)->GetDoubleArrayElements(env, b_vector, 0);

clock_gettime( CLOCK_MONOTONIC, &all_start);

        dCreate_CompCol_Matrix(&A, rows, cols, vals_size, values_double, rows_int, cols_int, SLU_NC, SLU_D, SLU_GE);
        dCreate_Dense_Matrix(&B, rows, 1,b_double, b_size, SLU_DN, SLU_D, SLU_GE);
        /*{
            double *sol = (double*) ((DNformat*) B.Store)->nzval;
            for(int i =0; i < n; ++i) {
                printf("[%d] < %f  \t > %f\n", i,b_double[i], sol[i]  );
            }
        }*/

        set_default_options(&options);
        xact = doubleMalloc(n * nrhs);
      ldx = n;
      dGenXtrue(n, nrhs, xact, ldx);
      //dFillRHS(options.Trans, nrhs, xact, ldx, &A, &B);

      if ( !(perm_c = intMalloc(n)) ) return;
      if ( !(perm_r = intMalloc(m)) ) return;

      /* Initialize the statistics variables. */
      StatInit(&stat);

clock_gettime( CLOCK_MONOTONIC, &solve_start);
      dgssv(&options, &A, perm_c, perm_r, &L, &U, &B, &stat, &info);

clock_gettime( CLOCK_MONOTONIC, &solve_stop);
//printf("info = %d\n", info);
      if ( info == 0 ) {
        // copy
        double *sol = (double*) ((DNformat*) B.Store)->nzval;
        /*for(int i=0; i < 10; ++i) {
            printf("[%d] -> %f____\n", i, sol[i]);
        }
        for(int i=b_size-10; i < b_size; ++i) {
                    printf("[%d] -> %f____\n", i, sol[i]);
                }
                */
        memcpy(b_double,sol, b_size*sizeof(double));
      }


    (*env)->ReleaseDoubleArrayElements(env, values, values_double, 0);
    (*env)->ReleaseDoubleArrayElements(env, b_vector, b_double, 0);
    (*env)->ReleaseIntArrayElements(env, rowPointed, rows_int, 0);
    (*env)->ReleaseIntArrayElements(env, colsPtrs, cols_int, 0);

clock_gettime( CLOCK_MONOTONIC, &all_stop);
/*
printf("All time = %f\n only solution = %f\n",
	all_stop.tv_sec - all_start.tv_sec + 1e-9 * (all_stop.tv_nsec - all_start.tv_nsec),
	solve_stop.tv_sec - solve_start.tv_sec + 1e-9 * (solve_stop.tv_nsec - solve_start.tv_nsec)
	);*/
	return;
#endif
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUMTWrapper_ccs_1components_1b_1pdgssv
  (JNIEnv *env, jclass cls, jint nproc, jint rows, jint cols, jintArray colsPtrs, jintArray rowPointed, jdoubleArray values, jdoubleArray b_vector)
{   SuperMatrix   A;
     NCformat *Astore;
     double   *a;
     int_t      *asub, *xa;
     int_t      *perm_r; /* row permutations from partial pivoting */
     int_t      *perm_c; /* column permutation vector */
     SuperMatrix   L;       /* factor L */
     SCPformat *Lstore;
     SuperMatrix   U;       /* factor U */
     NCPformat *Ustore;
     SuperMatrix   B;
int      nrhs, ldx, m, n, nnz;
    int_t info;

    int_t      panel_size, relax, maxsup;
    int_t      permc_spec;
    trans_t  trans;
    double   *xact, *rhs;
    superlu_memusage_t   superlu_memusage;
    int i;

    trans             = NOTRANS;
        n                 = 1000;
        panel_size        = 8; sp_ienv(1);
        relax             = 1; sp_ienv(2);
        maxsup            = 200; sp_ienv(3);
    jsize vals_size, cols_size, row_size, b_size;

    fprintf(stderr, "Called %s\n", __func__ );
    vals_size = (*env)->GetArrayLength(env, values);
    cols_size = (*env)->GetArrayLength(env, colsPtrs);
    row_size = (*env)->GetArrayLength(env, rowPointed);
    b_size = (*env)->GetArrayLength(env, b_vector);

    nnz = vals_size;
    m = rows;
    n = cols;
    nrhs = 1;

    jdouble *values_double = (*env)->GetDoubleArrayElements(env, values, 0);
    jint *rows_int = (*env)->GetIntArrayElements(env, rowPointed, 0);
    jint *cols_int = (*env)->GetIntArrayElements(env, colsPtrs, 0);
    jdouble *b_double = (*env)->GetDoubleArrayElements(env, b_vector, 0);

    int_t *rowsIntArray = (int_t*)calloc(row_size, sizeof(int_t));
    int_t *colsIntArray = (int_t*)calloc(cols_size, sizeof(int_t));
    for (i=0; i < row_size; ++i) {
        rowsIntArray[i] = rows_int[i];
    }
    for (i=0; i < cols_size; ++i) {
        colsIntArray[i] = cols_int[i];
    }
    (*env)->ReleaseIntArrayElements(env, rowPointed, rows_int, 0);
    (*env)->ReleaseIntArrayElements(env, colsPtrs, cols_int, 0);
    

 dCreate_CompCol_Matrix(&A, rows, cols, vals_size, values_double, rowsIntArray, colsIntArray, SLU_NC, SLU_D, SLU_GE);
    Astore = A.Store;
    printf("Dimension " IFMT "x" IFMT "; # nonzeros " IFMT "\n", A.nrow, A.ncol, Astore->nnz);
dCreate_Dense_Matrix(&B, rows, 1,b_double, b_size, SLU_DN, SLU_D, SLU_GE);
xact = doubleMalloc(n * nrhs);
    ldx = n;
    dGenXtrue(n, nrhs, xact, ldx);
     if (!(perm_r = intMalloc(m))) SUPERLU_ABORT("Malloc fails for perm_r[].");
        if (!(perm_c = intMalloc(n))) SUPERLU_ABORT("Malloc fails for perm_c[].");

            permc_spec = 1;
            get_perm_c(permc_spec, &A, perm_c);
printf("calling pdgssv\n");
            pdgssv(nproc, &A, perm_c, perm_r, &L, &U, &B, &info);
          if ( info == 0 ) {
          printf("info is 0\n");
      	    dinf_norm_error(nrhs, &B, xact); /* Inf. norm of the error */
            double *sol = (double*) ((DNformat*) B.Store)->nzval;
            memcpy(b_double,sol, b_size*sizeof(double));
            Lstore = (SCPformat *) L.Store;
        Ustore = (NCPformat *) U.Store;
            printf("#NZ in factor L = " IFMT "\n", Lstore->nnz);
            printf("#NZ in factor U = " IFMT "\n", Ustore->nnz);
            printf("#NZ in L+U = " IFMT "\n", Lstore->nnz + Ustore->nnz - L.ncol);

        superlu_dQuerySpace(nproc, &L, &U, panel_size, &superlu_memusage);
        printf("L\\U MB %.3f\ttotal MB needed %.3f\texpansions " IFMT "\n",
               superlu_memusage.for_lu/1024/1024,
               superlu_memusage.total_needed/1024/1024,
               superlu_memusage.expansions);

        } else {
            printf("info is " IFMT "\n", info);
        }

            (*env)->ReleaseDoubleArrayElements(env, values, values_double, 0);
            (*env)->ReleaseDoubleArrayElements(env, b_vector, b_double, 0);
            //(*env)->ReleaseIntArrayElements(env, rowPointed, rows_int, 0);
            //(*env)->ReleaseIntArrayElements(env, colsPtrs, cols_int, 0);
    free(rowsIntArray);
    free(colsIntArray);

        SUPERLU_FREE (xact);
        SUPERLU_FREE (perm_r);
        SUPERLU_FREE (perm_c);
        Destroy_SuperMatrix_Store(&A);
        Destroy_SuperMatrix_Store(&B);
        Destroy_SuperNode_SCP(&L);
        Destroy_CompCol_NCP(&U);
}
