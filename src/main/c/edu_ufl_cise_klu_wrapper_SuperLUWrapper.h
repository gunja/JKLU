/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class edu_ufl_cise_klu_wrapper_SuperLUWrapper */

#ifndef _Included_edu_ufl_cise_klu_wrapper_SuperLUWrapper
#define _Included_edu_ufl_cise_klu_wrapper_SuperLUWrapper
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    dgssv
 * Signature: (Ledu/ufl/cise/klu/wrapper/superlu_options_t;Ledu/ufl/cise/klu/wrapper/SuperMatrix;[I[ILedu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperLUStat_t;)I
 */
JNIEXPORT jint JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dgssv
  (JNIEnv *, jclass, jobject, jobject, jintArray, jintArray, jobject, jobject, jobject, jobject);

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    ccs_components_b_dgssv
 * Signature: (II[I[I[D[D)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_ccs_1components_1b_1dgssv
  (JNIEnv *, jclass, jint, jint, jintArray, jintArray, jdoubleArray, jdoubleArray);

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    dGenXtrue
 * Signature: (III)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dGenXtrue
  (JNIEnv *, jclass, jint, jint, jint);

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    dFillRHS
 * Signature: (Ledu/ufl/cise/klu/wrapper/trans_t;I[DILedu/ufl/cise/klu/wrapper/SuperMatrix;Ledu/ufl/cise/klu/wrapper/SuperMatrix;)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dFillRHS
  (JNIEnv *, jclass, jobject, jint, jdoubleArray, jint, jobject, jobject);

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    get_perm_c
 * Signature: (ILedu/ufl/cise/klu/wrapper/SuperMatrix;[I)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_get_1perm_1c
  (JNIEnv *, jclass, jint, jobject, jintArray);

/*
 * Class:     edu_ufl_cise_klu_wrapper_SuperLUWrapper
 * Method:    dinf_norm_error
 * Signature: (ILedu/ufl/cise/klu/wrapper/SuperMatrix;[D)V
 */
JNIEXPORT void JNICALL Java_edu_ufl_cise_klu_wrapper_SuperLUWrapper_dinf_1norm_1error
  (JNIEnv *, jclass, jint, jobject, jdoubleArray);

#ifdef __cplusplus
}
#endif
#endif
