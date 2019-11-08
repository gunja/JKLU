#include "edu_ufl_cise_klu_wrapper_SuperLUWrapper.h"
#include <stdio.h>
#include "slu_ddefs.h"

#include <time.h>

JNIEXPORT jdoubleArray JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dGenXtrue
  (JNIEnv *env, jclass cls, jint sz, jint sz2, jint sz3)
{
	printf("Called %s\n", __func__ );
	//TODO;
    return (*env)->NewGlobalRef(env, NULL);
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dFillRHS
  (JNIEnv *env, jclass cls, jobject obj, jint sz, jdoubleArray dArra, jint sz2,
	jobject obj1, jobject obj2)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_get_1perm_1c
  (JNIEnv *env, jclass cls, jint sz, jobject obj1, jintArray iArra)
{
	printf("Called %s\n", __func__ );
	return;
}

void static updateInternalFieldSM_L(JNIEnv *env, jobject SMJava, SuperMatrix* native) {
    jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
    jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");
    jfieldID fInited = (*env)->GetFieldID(env, matCLS, "isInitialized", "Z");
    // if we were passed with null object - nowhere to store
    // otherwise, delete previous object, and store new one
    if( !(*env)->IsSameObject(env, SMJava, NULL) )
    {
        jobject intra = (*env)->GetObjectField(env, SMJava, fid);
        if(!(*env)->IsSameObject(env, intra, NULL) ) {
            SuperMatrix *oldL = (*env)->GetDirectBufferAddress(env, intra);
            Destroy_SuperNode_Matrix(oldL);
        }
        intra = (*env)->NewDirectByteBuffer(env, native, sizeof(*native));
        (*env)->SetObjectField(env, SMJava, fid, intra);
        (*env)->SetBooleanField(env, SMJava, fInited, 1);
    }
}

void static updateInternalFieldSM_U(JNIEnv *env, jobject SMJava, SuperMatrix* native) {
    jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
    jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");
    jfieldID fInited = (*env)->GetFieldID(env, matCLS, "isInitialized", "Z");
    // if we were passed with null object - nowhere to store
    // otherwise, delete previous object, and store new one
    if( !(*env)->IsSameObject(env, SMJava, NULL) )
    {
        jobject intra = (*env)->GetObjectField(env, SMJava, fid);
        if( !(*env)->IsSameObject(env, intra, NULL) ) {
            SuperMatrix *oldL = (*env)->GetDirectBufferAddress(env, intra);
            Destroy_CompCol_Matrix(oldL);
        }
        intra = (*env)->NewDirectByteBuffer(env, native, sizeof(*native));
        (*env)->SetObjectField(env, SMJava, fid, intra);
        (*env)->SetBooleanField(env, SMJava, fInited, 1);
    }
}
// static
/*
* Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
* Method:    dgssv
* Signature: (Ledu/ufl/cise/klu/wrapper/superlu_options_t;Ledu/ufl/cise/klu/wrapper/SuperMatrix;[I[ILedu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperLUStat_t;Ljava/lang/Integer;)V
*/
JNIEXPORT int JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dgssv
(JNIEnv *env, jclass SLUcls, jobject optionsIgnored, jobject A, jintArray permC, jintArray permR, jobject L, jobject U, jobject B, jobject stats)
{
    jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
    jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");

	//printf("Called %s\n", __func__ );
	SuperMatrix  *A_, *L_, *U_, *B_;
	int info;
	SuperLUStat_t stat;


	L_ = (SuperMatrix*)malloc( sizeof(SuperMatrix));
	memset(L_, 0, sizeof(SuperMatrix));
    U_ = (SuperMatrix*)malloc( sizeof(SuperMatrix));
	memset(U_, 0, sizeof(SuperMatrix));
	// B -matrix should be filled previously for us
    //B_ = (SuperMatrix*)malloc( sizeof(SuperMatrix));
	//memset(B_, 0, sizeof(SuperMatrix));
	updateInternalFieldSM_L(env, L, L_);
	updateInternalFieldSM_U(env, U, U_);

	jobject intra = (*env)->GetObjectField(env, B, fid);
	B_ = (*env)->GetDirectBufferAddress(env, intra);

	intra = (*env)->GetObjectField(env, A, fid);
    A_ = (*env)->GetDirectBufferAddress(env, intra);

    superlu_options_t options;
    set_default_options(&options);

    // assume that permC and permR are already properly allocated
    int internalPermutationC = 0;
    int internalPermutationR = 0;
    int *perm_c, *perm_r;

    if( (*env)->IsSameObject(env, permC, NULL) ) {
        internalPermutationC = 1;
        perm_c = (int*)calloc(A_->ncol, sizeof(int));
    } else {
        perm_c = (*env)->GetIntArrayElements(env, permC, NULL);
    }

    if( (*env)->IsSameObject(env, permR, NULL) ) {
        internalPermutationR = 1;
        perm_r = (int*)calloc(A_->nrow, sizeof(int));
    } else {
        perm_r = (*env)->GetIntArrayElements(env, permR, NULL);
    }

    StatInit(&stat);
    dgssv(&options, A_, perm_c, perm_r, L_, U_, B_, &stat, &info);

    if(internalPermutationC) {
        free(perm_c);
    } else {
        (*env)->ReleaseIntArrayElements(env, permC, perm_c, 0);
    }
    if(internalPermutationR) {
        free(perm_r);
    } else {
        (*env)->ReleaseIntArrayElements(env, permR, perm_r, 0);
    }
	return info;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dinf_1norm_1error
  (JNIEnv *env, jclass cls, jint sz, jobject obj1, jdoubleArray dArra)
{
	printf("Called %s\n", __func__ );
	return;
}

JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_ccs_1components_1b_1dgssv
  (JNIEnv *env, jclass cld, jint rows, jint cols, jintArray colsPtrs, jintArray rowPointed,
	jdoubleArray values, jdoubleArray b_vector)
{
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
}
