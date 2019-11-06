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

