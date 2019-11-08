#include "edu_ufl_cise_klu_wrapper_SuperMatrix.h"
#include "slu_ddefs.h"

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
  (JNIEnv *env, jobject obj, jint col, jint row)
{
     jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
     jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");
     jfieldID fInited = (*env)->GetFieldID(env, matCLS, "isInitialized", "Z");

     int val = (*env)->GetBooleanField(env, obj, fInited);
     if (val == 0) {
        // throw
        return 0;
     }
     jobject intra = (*env)->GetObjectField(env, obj, fid);
     SuperMatrix *m = (*env)->GetDirectBufferAddress(env, intra);
     if(col <0 || col >= m->ncol) {
        return 0.;
     }
     if(row < 0 || row >= m->nrow) {
        return 0.;
     }
     if(m->Stype == SLU_DN) {
        DNformat * store = (DNformat*) m->Store;
        double * firstElement = (double*)store->nzval;
        return firstElement[col * store->lda + row];
     }
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
    SuperMatrix *A = ( SuperMatrix *) (*env)->GetDirectBufferAddress(env, mat);
    //Destroy_SuperMatrix_Store(A);
    Destroy_CompCol_Matrix(A);
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
     jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
     jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");
     jfieldID fInited = (*env)->GetFieldID(env, matCLS, "isInitialized", "Z");

     int val = (*env)->GetBooleanField(env, obj, fInited);
     if (val == 0) {
        // throw
        return 0;
     }
     jobject intra = (*env)->GetObjectField(env, obj, fid);
     SuperMatrix *m = (*env)->GetDirectBufferAddress(env, intra);

    return m->ncol;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    rows_native
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_rows_1native
  (JNIEnv *env, jobject obj)
{
     jclass matCLS = (*env)->FindClass(env, "edu/ufl/cise/klu/wrapper/SuperMatrix");
     jfieldID fid = (*env)->GetFieldID(env, matCLS, "SuperMatrix_nativePtr", "Ljava/lang/Object;");
     jfieldID fInited = (*env)->GetFieldID(env, matCLS, "isInitialized", "Z");

     int val = (*env)->GetBooleanField(env, obj, fInited);
     if (val == 0) {
        // throw
        return 0;
     }
     jobject intra = (*env)->GetObjectField(env, obj, fid);
     SuperMatrix *m = (*env)->GetDirectBufferAddress(env, intra);

    return m->nrow;
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

    jdouble *values_double = (*env)->GetDoubleArrayElements(env, NonZeroArray, 0);
    jint *rows_int = (*env)->GetIntArrayElements(env, rowInd, 0);
    jint *cols_int = (*env)->GetIntArrayElements(env, colPtr, 0);

    double *r_vals = (double*) calloc(vals_size, sizeof(double));
    int *r_rows = (int*) calloc(row_size, sizeof(int));
    int *r_cols = (int*) calloc(cols_size, sizeof(int));
    memcpy(r_vals, values_double, sizeof(double)* vals_size);
    memcpy(r_rows, rows_int, sizeof(int)*row_size);
    memcpy(r_cols, cols_int, sizeof(int)*cols_size);

    dCreate_CompCol_Matrix(A, rows, cols, vals_size, r_vals, r_rows, r_cols, SLU_NC, SLU_D, SLU_GE);

    (*env)->ReleaseDoubleArrayElements(env, NonZeroArray, values_double, 0);
    (*env)->ReleaseIntArrayElements(env, rowInd, rows_int, 0);
    (*env)->ReleaseIntArrayElements(env, colPtr, cols_int, 0);

    jmethodID constructor = (*env)->GetMethodID(env, SMcls, "<init>", "()V");
    jobject object = (*env)->NewObject(env, SMcls, constructor);

    jfieldID fid = (*env)->GetFieldID(env, SMcls, "isInitialized", "Z");
    (*env)->SetBooleanField(env, object, fid, 1);

    fid = (*env)->GetFieldID(env, SMcls, "SuperMatrix_nativePtr", "Ljava/lang/Object;");

    jobject intra = (*env)->NewDirectByteBuffer(env, A, sizeof(*A));
    (*env)->SetObjectField(env, object, fid, intra);

    return object;
}

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperMatrix
 * Method:    dCreateDenseMatrix
 * Signature: (II[DLedu/ufl/cise/klu/wrapper/Stype_t;Ledu/ufl/cise/klu/wrapper/Dtype_t;Ledu/ufl/cise/klu/wrapper/Mtype_t;)Ledu/ufl/cise/klu/wrapper/SuperMatrix;
 */
JNIEXPORT jobject JNICALL Java_edu_ufl_cise_klu_wrapper_SuperMatrix_dCreateDenseMatrix
  (JNIEnv *env, jclass SMcls, jint rows, jint cols, jdoubleArray values,  jobject stype, jobject dtype, jobject mtype)
{
	printf("Called %s\n", __func__ );

    SuperMatrix *B;
    B = (SuperMatrix*)malloc(sizeof(SuperMatrix));
    memset(B, 0, sizeof(SuperMatrix));


    jdouble *values_double = (*env)->GetDoubleArrayElements(env, values, 0);
        jsize vals_size = (*env)->GetArrayLength(env, values);
        double *r_values = (double*)calloc(vals_size, sizeof(double));
        memcpy(r_values, values_double, vals_size* sizeof(double));
    (*env)->ReleaseDoubleArrayElements(env, values, values_double, 0);

    // vals_size should be equal to rows

    dCreate_Dense_Matrix(B, rows, cols, r_values, rows, SLU_DN, SLU_D, SLU_GE);


    jmethodID constructor = (*env)->GetMethodID(env, SMcls, "<init>", "()V");
    jobject object = (*env)->NewObject(env, SMcls, constructor);

    jfieldID fid = (*env)->GetFieldID(env, SMcls, "isInitialized", "Z");
    (*env)->SetBooleanField(env, object, fid, 1);

    fid = (*env)->GetFieldID(env, SMcls, "SuperMatrix_nativePtr", "Ljava/lang/Object;");

    jobject intra = (*env)->NewDirectByteBuffer(env, B, sizeof(*B));
    (*env)->SetObjectField(env, object, fid, intra);

    return object;
}
