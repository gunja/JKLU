#include "edu_ufl_cise_klu_wrapper_SuperMatrix.h"

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    set_native
 * Signature: (IID)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_set_1native
  (JNIEnv * env, jobject obj, jint i, jint j, jdouble val)
{
    // TODO this is not needed at the moment
    return;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    get_native
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_get_1native
  (JNIEnv *env, jobject obj, jint i, jint j)
{
    return 0.0;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    release_CompCol_Matrix
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_release_1CompCol_1Matrix
  (JNIEnv *env, jobject obj, jobject mat)
{
    return;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    cols_native
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_cols_1native
  (JNIEnv * env, jobject obj)
{
    return 1;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    rows_native
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_rows_1native
  (JNIEnv *env, jobject obj)
{
    return 1;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    dCreateCompColMatrix
 * Signature: (II[D[I[ILedu/ufl/cise/klu/wrapper/Stype_t;Ledu/ufl/cise/klu/wrapper/Dtype_t;Ledu/ufl/cise/klu/wrapper/Mtype_t;)Ledu/ufl/cise/klu/wrapper/SuperMatrix;
 */
JNIEXPORT jobject JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_dCreateCompColMatrix
  (JNIEnv *env, jclass SMcls, jint rows, jint cols , jdoubleArray NonZeroArray, jintArray rowInd, jintArray colPtr, jobject stype, jobject dtype, jobject mtype)
{
	printf("Called %s\n", __func__ );

    SuperMatrix *A;
    A = (SuperMatrix*)malloc(sizeof(SuperMatrix));
    memset(A, 0, sizeof(SuperMatrix));

    jsize vals_size, cols_size, row_size;

    vals_size = (*env)->GetArrayLength(env, NonZeroArray);
    cols_size = (*env)->GetArrayLength(env, colPtr);
    row_size = (*env)->GetArrayLength(env, rowInd);

    nnz = vals_size;
    m = rows;
    n = cols;
    nrhs = 1;

    jdouble *values_double = (*env)->GetDoubleArrayElements(env, nonZeroes, 0);
    jint *rows_int = (*env)->GetIntArrayElements(env, rowInd, 0);
    jint *cols_int = (*env)->GetIntArrayElements(env, colPtr, 0);

    dCreate_CompCol_Matrix(&A, rows, cols, vals_size, values_double, rows_int, cols_int, SLU_NC, SLU_D, SLU_GE);

    (*env)->ReleaseDoubleArrayElements(env, NonZeroArray, values_double, 0);
    (*env)->ReleaseIntArrayElements(env, rowInd, rows_int, 0);
    (*env)->ReleaseIntArrayElements(env, colPtr, cols_int, 0);

    jmethodID constructor = (*env)->GetMethodID(env, SMcls, "<init>", "void(V)");
    jobject object = (*env)->NewObject(env, SMcls, constructor);

    jfieldID fid = (*env)->GetFieldID(env, SMcls, "isInitialized", "Z");
    (*env)->SetBooleanField(env, object, fid, true);


	//return NULL;
    return object;
}

