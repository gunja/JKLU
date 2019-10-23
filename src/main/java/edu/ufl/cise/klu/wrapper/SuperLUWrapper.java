package edu.ufl.cise.klu.wrapper;

//import edu.ufl.cise.klu.wrapper.SuperMatrix;
import edu.ufl.cise.klu.wrapper.SuperMatrix;

public class SuperLUWrapper {
    static {
        System.loadLibrary("superLUWrapper");
    }

    public static native void dCreate_CompCol_Matrix(SuperMatrix A, int m, int n, int nnz, double[] nzval, int[] rowind, int[] colptr, Stype_t stype, Dtype_t dtype, Mtype_t mtype);
    public static native void dGenXtrue (int n, int nrhs, double[] x, int Idx);
    public static native void dFillRHS (trans_t trans, int nrhs, double[] x, int ldx, SuperMatrix A, SuperMatrix B);
    public static native void get_perm_c(int ispec, SuperMatrix A, int[] perm_c);
    public static native void dgssv( superlu_options_t[]  	options, SuperMatrix A, int[] perm_c, int[] perm_r, SuperMatrix	L,
                              SuperMatrix U, SuperMatrix B, SuperLUStat_t[] stat, int[] info);
    public static native void dinf_norm_error(int nrhs, SuperMatrix X, double[] xtrue);
    public static native void ccs_components_b_dgssv( int rows, int cols, int[] colPointers, int[] rowPointers, double[] values, double[] b );
}