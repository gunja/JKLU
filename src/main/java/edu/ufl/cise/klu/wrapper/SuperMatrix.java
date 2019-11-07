package edu.ufl.cise.klu.wrapper;


public class SuperMatrix {
    static {
        System.loadLibrary("superLUWrapper");
    }

    private boolean isInitialized = false;
    private Object SuperMatrix_nativePtr = null;
    private native void set_native(int col, int row, double value);
    private native double get_native(int col, int row);
    private native void release_CompCol_Matrix(Object nativePtr);
    private native int cols_native();
    private native int rows_native();

    //not good since Java 9, but let's use it for time being
    @Override
    protected void finalize() throws Throwable {
        try {
             if (isInitialized) {
                 close();
                 // TODO clear resources
             }
        } finally {
            super.finalize();
        }
    }
    public SuperMatrix(){
    }

    public static native SuperMatrix dCreateCompColMatrix(int m, int n, double[] nzval, int[] rowind, int[] colptr, Stype_t stype, Dtype_t dtype, Mtype_t mtype);

    public void close(){
        if (isInitialized) {
            release_CompCol_Matrix(SuperMatrix_nativePtr);
            isInitialized = false;
        }
    }

    public void set(int col, int row, double value) {
        if (!isInitialized) {
            return;
        }
        set_native(col, row, value);
    }

    public double get(int col, int row) {
        if(isInitialized) {
            return get_native(col, row);
        }
        return 0.;
    }

    public int rows() {
        if (isInitialized) {
            return rows_native();
        }
        return 0;
    }

    public int cols() {
        if (isInitialized) {
            return cols_native();
        }
        return 0;
    }


}
