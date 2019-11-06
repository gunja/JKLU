package edu.ufl.cise.klu.wrapper;

public class SuperLUWrapper {
    static {
        System.loadLibrary("superLUWrapper");
    }

    // m and n are col counter of resulting matrix
    // nzval - array of non-zeroes. length is obtained internally
    // rowind - array of row pointers
    // colptr - array of column pointers
    // stype, dtype and mtype are ignored (set internally to proper values
    public static native  SuperMatrix dCreateCompColMatrix(int m, int n, double[] nzval, int[] rowind, int[] colptr, Stype_t stype, Dtype_t dtype, Mtype_t mtype);
    // options are ignored (set to default
    public static native void dgssv( superlu_options_t	options, SuperMatrix A, int[] perm_c, int[] perm_r, SuperMatrix	L,
                                     SuperMatrix U, SuperMatrix B, SuperLUStat_t stat, Integer info);
    //for comparison of results
    public static native void ccs_components_b_dgssv( int rows, int cols, int[] colPointers, int[] rowPointers, double[] values, double[] b );




    public static native double[] dGenXtrue (int n, int nrhs, int Idx);
    public static native void dFillRHS (trans_t trans, int nrhs, double[] x, int ldx, SuperMatrix A, SuperMatrix B);
    public static native void get_perm_c(int ispec, SuperMatrix A, int[] perm_c);


    public static native void dinf_norm_error(int nrhs, SuperMatrix X, double[] xtrue);

}

