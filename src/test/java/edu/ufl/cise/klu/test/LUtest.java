package edu.ufl.cise.klu.test;

import edu.ufl.cise.klu.common.KLU_common;
import edu.ufl.cise.klu.common.KLU_numeric;
import edu.ufl.cise.klu.common.KLU_symbolic;
import edu.ufl.cise.klu.utils.CCSMatrixWrap;
import edu.ufl.cise.klu.wrapper.*;
import no.uib.cipr.matrix.Matrix;
import no.uib.cipr.matrix.sparse.CompColMatrix;
import no.uib.cipr.matrix.sparse.FlexCompColMatrix;
import no.uib.cipr.matrix.sparse.SparseVector;
import org.la4j.Vector;
import org.la4j.iterator.VectorIterator;
import org.la4j.matrix.DenseMatrix;
import org.la4j.matrix.SparseMatrix;
import org.la4j.matrix.sparse.CCSMatrix;
import org.la4j.matrix.sparse.CRSMatrix;

import static edu.ufl.cise.klu.tdouble.Dklu_analyze.klu_analyze;
import static edu.ufl.cise.klu.tdouble.Dklu_defaults.klu_defaults;
import static edu.ufl.cise.klu.tdouble.Dklu_factor.klu_factor;
import static edu.ufl.cise.klu.tdouble.Dklu_solve.klu_solve;
import static edu.ufl.cise.klu.tdouble.Dklu_version.KLU_SINGULAR;


import java.lang.reflect.Field;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Random;
import java.util.StringTokenizer;

public class LUtest {

    private static final double DELTA = 1e-09 ;

    private static int n = 4000 ;
    private static int [ ] Ap = {0, 2, 5, 9, 10, 12} ;
    private static int [ ] Ai = {0, 1, 0, 2, 4, 1, 2, 3, 4, 2, 1, 4} ;
    private static double [ ] Ax = {2., 3., 3., -1., 4., 4., -3., 1., 2., 2., 6., 1.} ;
    private static double [ ] b = {8., 45., -3., 3., 19.} ;

    /**
     * a simple KLU demo; solution is x = (1,2,3,4,5)
     */

    public static void fillVectors(SparseMatrix sm, int[] Ap, int[] Ai, double[] Ax)
    {


        int sum =0, c;
        int R=0;
        for(c=0; c < sm.columns(); ++c) {
            //for(c=0; c < 10; ++c) {
            Ap[c] = sum;
            int rr=0;
            VectorIterator i = sm.nonZeroIteratorOfColumn(c);
            //System.out.println("ColNum =" + c );
            for(i.next(); i.hasNext(); i.next())
            {
                if(Math.abs(i.get()) > DELTA) {
                    //System.out.printf("value at sum=%d    %.7f\n", sum, i.get());
                    Ax[sum] = i.get();
                    Ai[sum] = i.index();
                    sum++;
                }
            }
            if(Math.abs(i.get()) > DELTA) {
                Ax[sum] = i.get();
                Ai[sum] = i.index();
                sum++;
            }
        }
        Ap[c] = sum;
    }

    public static void mtj_ccs_klu_test(){
        System.out.println("\nstarting mtj_ccs_klu_test\n");
        long startTime = System.nanoTime();

        FlexCompColMatrix sparseMatrix = new FlexCompColMatrix(n,n);
        CompColMatrix tst = new CompColMatrix(sparseMatrix);

        System.out.println("Matrix instantiated time msec: "+ (System.nanoTime()-startTime)/1000000);

        startTime = System.nanoTime();

        b= new double[n];

        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.numColumns();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.numRows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
        }
        System.out.println("Matrix initialized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        CompColMatrix ccsm = new CompColMatrix(sparseMatrix);

//        new CompColMatrix()

        Ap= ccsm.getColumnPointers();
        Ai= ccsm.getRowIndices();
        Ax= ccsm.getData();

        System.out.println("Retrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        KLU_symbolic Symbolic;
        KLU_numeric Numeric;
        KLU_common Common = new KLU_common();

        //Dklu_version.NPRINT = false ;
        //Dklu_internal.NDEBUG = false ;

        klu_defaults (Common);


        Symbolic = klu_analyze (n, Ap, Ai, Common);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        Numeric = klu_factor (Ap, Ai, Ax, Symbolic, Common);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (Symbolic, Numeric, n, 1, b, 0, Common);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = 0 ; i < n ; i++) {
            System.out.printf("x [%d] = %g\n", i, b [i]) ;
////            assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    public static void la4j_ccs_klu_test(){
        System.out.println("\nla4j_ccs_klu_test\n");
        long startTime = System.nanoTime();
        SparseMatrix sparseMatrix = CCSMatrix.zero(n, n);
        System.out.println("Matrix instantiated time msec: "+ (System.nanoTime()-startTime)/1000000);

        startTime = System.nanoTime();

        b= new double[n];

        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.rows();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.rows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
        }
        System.out.println("Matrix initialized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        Ap= CCSMatrixWrap.getColumnPointers(sparseMatrix);
        Ai= CCSMatrixWrap.getRowIndices(sparseMatrix);
        Ax= CCSMatrixWrap.getValues(sparseMatrix);


        System.out.println("Retrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        KLU_symbolic Symbolic;
        KLU_numeric Numeric;
        KLU_common Common = new KLU_common();

        //Dklu_version.NPRINT = false ;
        //Dklu_internal.NDEBUG = false ;

        klu_defaults (Common);


        Symbolic = klu_analyze (n, Ap, Ai, Common);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        Numeric = klu_factor (Ap, Ai, Ax, Symbolic, Common);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (Symbolic, Numeric, n, 1, b, 0, Common);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = n-10 ; i < n ; i++) {
            System.out.printf("x [%d] = %g\n", i, b [i]) ;
////            assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    private static void superluwr_ccs_klu_test()
    {
        System.out.println(""); System.out.println("starting test with SuperLU native");
        long startTime = System.nanoTime();


        Stype_t stype = new Stype_t();
        Dtype_t dtype = new Dtype_t();
        Mtype_t mtype = new Mtype_t();
        superlu_options_t options = null;

        SparseMatrix sparseMatrix = CCSMatrix.zero(n, n);
        System.out.println("Matrix instantiated time msec: "+ (System.nanoTime()-startTime)/1000000);

        startTime = System.nanoTime();

        b= new double[n];

        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.rows();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.rows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
        }
        System.out.println("Matrix initialized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        Ap= CCSMatrixWrap.getColumnPointers(sparseMatrix);
        Ai= CCSMatrixWrap.getRowIndices(sparseMatrix);
        Ax= CCSMatrixWrap.getValues(sparseMatrix);

        SuperMatrix superMatrixA = SuperMatrix.dCreateCompColMatrix(n, n,  Ax, Ai, Ap, stype, dtype, mtype);


        System.out.println("Retrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        KLU_symbolic Symbolic;
        KLU_numeric Numeric;
        KLU_common Common = new KLU_common();

        //Dklu_version.NPRINT = false ;
        //Dklu_internal.NDEBUG = false ;

        klu_defaults (Common);


        Symbolic = klu_analyze (n, Ap, Ai, Common);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        Numeric = klu_factor (Ap, Ai, Ax, Symbolic, Common);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (Symbolic, Numeric, n, 1, b, 0, Common);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = 0 ; i < n ; i++) {
            System.out.printf("x [%d] = %g\n", i, b [i]) ;
////            assertEquals(i + 1.0, b [i], DELTA) ;
        }

        int[] perm_c = new int[n];
        int[] perm_r = new int[n];
        SuperMatrix L = new SuperMatrix();
        SuperMatrix U = new SuperMatrix();
        SuperMatrix B = new SuperMatrix();
        SuperLUStat_t stat = null;
        Integer info = new Integer(0);
        SuperLUWrapper.dgssv(options, superMatrixA, perm_c, perm_r,  L, U, B, stat, info);
        //after that somehow get B->vector of solution
        // TODO ^^^^^^

    }

    private static void short_superLU_wrapped_ccs_test()
    {
        //partial copy ov la4j_ccs_klu

        long startTime = System.nanoTime();
        SparseMatrix sparseMatrix = CCSMatrix.zero(n, n);
        System.out.println("[LA4J]  Matrix instantiated time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        b= new double[n];
        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.rows();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.rows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
        }
        System.out.println("Matrix initialized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        Ap= CCSMatrixWrap.getColumnPointers(sparseMatrix);
        Ai= CCSMatrixWrap.getRowIndices(sparseMatrix);
        Ax= CCSMatrixWrap.getValues(sparseMatrix);

        System.out.println("Retrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        SuperLUWrapper.ccs_components_b_dgssv(n,n, Ap, Ai, Ax, b);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        //System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = n- 10 ; i < n ; i++) {
            System.out.printf("x [%d] = %g\n", i, b [i]) ;
            //assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    private static void short_superLU_wrapped_ccs_test_par()
    {
        //partial copy ov la4j_ccs_klu

        long startTime = System.nanoTime();
        SparseMatrix sparseMatrix = CCSMatrix.zero(n, n);
        System.out.println("[LA4J] Parallel Matrix instantiated time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        b= new double[n];
        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.rows();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.rows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
        }
        System.out.println("Matrix initialized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        Ap= CCSMatrixWrap.getColumnPointers(sparseMatrix);
        Ai= CCSMatrixWrap.getRowIndices(sparseMatrix);
        Ax= CCSMatrixWrap.getValues(sparseMatrix);

        System.out.println("Retrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();

        SuperLUMTWrapper.ccs_components_b_pdgssv(4, n,n, Ap, Ai, Ax, b);

        System.out.println("Par superLU wrapped Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        //System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = n-10 ; i < n ; i++) {
            System.out.printf("x [%d] = %g\n", i, b [i]) ;
            //assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    private static void dump_matrix()
    {
        SparseMatrix sparseMatrix = CCSMatrix.zero(n, n);
        long startTime = System.nanoTime();
        System.out.println("Dump matrix started");
        b= new double[n];
        double[] myb= new double[n];
        Random rand = new Random();
        rand.setSeed(1212);

        for(int i=0;i<sparseMatrix.rows();i++){
            for(int m=0;m<rand.nextInt(50);m++){
                int k = rand.nextInt(sparseMatrix.rows());
                double val = rand.nextDouble();
                sparseMatrix.set(i,k,val);
                sparseMatrix.set(k,i,val);
                sparseMatrix.set(i,i,rand.nextDouble());
            }

            b[i]=rand.nextDouble();
            myb[i] = b[i];
        }
        System.out.println("loaded matrix with cardinality " + sparseMatrix.cardinality() +"    Columns = " + sparseMatrix.columns() + "    rows =" + sparseMatrix.rows() );

        startTime = System.nanoTime();
        Ap= CCSMatrixWrap.getColumnPointers(sparseMatrix);
        Ai= CCSMatrixWrap.getRowIndices(sparseMatrix);
        Ax= CCSMatrixWrap.getValues(sparseMatrix);
        System.out.println("\nRetrieved CCS data time msec: "+ (System.nanoTime()-startTime)/1000000);

        System.out.println("loaded matrix Ap.size= " + Ap.length +"    Ai.size = " + Ai.length + "    Ax =" + Ax.length );
        int j, c =0;
        for (j=0; j < Ax.length; ++j) {
            if ( Math.abs(Ax[j]) < 1e-7) c++;
        }
        System.out.println("C=" + c + "   real card=" + (sparseMatrix.cardinality() + c) + "   vs   "+  Ax.length);

        startTime = System.nanoTime();
        int[] myAp = new int[sparseMatrix.columns()+1];
        int[] myAi = new int[sparseMatrix.cardinality()];
        double[] myAx = new double[sparseMatrix.cardinality()];
        fillVectors(sparseMatrix, myAp, myAi, myAx);
        System.out.println("\nRetrieved data in direct mode data time msec: "+ (System.nanoTime()-startTime)/1000000);
        for(c=0; c < 10; ++c) {
            System.out.println("myAx["+c+"] ="+ myAx[c] + "    vs    Ax ->"+ Ax[c]);
        }
        for(c=0; c < 10; ++c) {
            System.out.println("myAi["+c+"] ="+ myAi[c] + "    vs    Ai ->"+ Ai[c]);
        }
        for(c=0; c < 10; ++c) {
            System.out.println("myAp["+c+"] ="+ myAp[c] + "    vs    Ap ->"+ Ap[c]);
        }

        startTime = System.nanoTime();

        KLU_symbolic Symbolic;
        KLU_numeric Numeric;
        KLU_common Common = new KLU_common();
        klu_defaults (Common);

        Symbolic = klu_analyze (n, Ap, Ai, Common);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        Numeric = klu_factor (Ap, Ai, Ax, Symbolic, Common);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (Symbolic, Numeric, n, 1, b, 0, Common);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);

        System.out.println("Restarting : with less ");
        startTime = System.nanoTime();

        KLU_symbolic mySymbolic;
        KLU_numeric myNumeric;
        KLU_common myCommon = new KLU_common();
        klu_defaults (myCommon);


        mySymbolic = klu_analyze (n, myAp, myAi, myCommon);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        myNumeric = klu_factor (myAp, myAi, myAx, mySymbolic, myCommon);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (mySymbolic, myNumeric, n, 1, myb, 0, myCommon);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ myCommon.mempeak+" status "+myCommon.status);
        for (int i = 0 ; i < n ; i++) {
            System.out.printf("x[%d] =  %g     my  = %g\n", i, b[i], myb [i]) ;
////            assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    private static void la4j_ccs_klu_test_rajat()
    {

    }

    private static void superLU_par_wrapped_ccs_test_rajat()
    {
        System.out.println("\n\nStarting superLU_par_wrapped_ccs_test_rajat\n");

        String fileContent = null;
        long startTime = System.nanoTime();
        System.out.println(System.getProperty("user.dir"));
        try {
            fileContent = new String(Files.readAllBytes(Paths.get("rajat22.mtx")), StandardCharsets.UTF_8);
        }
        catch(Throwable t) {
            System.out.println("Exception while reading file");
            return;
        }


        SparseMatrix sparseMatrix = SparseMatrix.fromMatrixMarket(fileContent);
        System.out.println("loaded matrix with cardinality " + sparseMatrix.cardinality());
        System.out.println("Columns = " + sparseMatrix.columns() + "    rows =" + sparseMatrix.rows());

        n = sparseMatrix.columns();
        Ap = new int[sparseMatrix.columns()+1];
        Ai = new int[sparseMatrix.cardinality()];
        Ax = new double[sparseMatrix.cardinality()];

        int[] myAp = new int[sparseMatrix.columns()+1];
        int[] myAi = new int[sparseMatrix.cardinality()];
        double[] myAx = new double[sparseMatrix.cardinality()];

        System.out.print("Filling matrices vectors...");
        startTime = System.nanoTime();
        fillVectors(sparseMatrix, Ap, Ai, Ax);
        System.out.print("   First part done in " +  (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        fillVectors(sparseMatrix, myAp, myAi, myAx);
        System.out.println("   Second part done in " +  (System.nanoTime()-startTime)/1000000);

        Random rand = new Random();
        rand.setSeed(1212);
        double[] b = new double[sparseMatrix.columns()];
        double[] myb = new double[sparseMatrix.columns()];
        for(int i=0; i < sparseMatrix.columns(); ++i) {
            b[i]=rand.nextDouble();
            myb[i] = b[i];
        }

        System.out.println("\n\ndone preparation\n");

        KLU_symbolic Symbolic;
        KLU_numeric Numeric;
        KLU_common Common = new KLU_common();
        klu_defaults (Common);

        Symbolic = klu_analyze (n, Ap, Ai, Common);
        System.out.println("Klu analyzed time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        Numeric = klu_factor (Ap, Ai, Ax, Symbolic, Common);
        System.out.println("Klu factorized time msec: "+ (System.nanoTime()-startTime)/1000000);
        startTime = System.nanoTime();
        klu_solve (Symbolic, Numeric, n, 1, b, 0, Common);

        System.out.println("Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);

        startTime = System.nanoTime();

        SuperLUMTWrapper.ccs_components_b_pdgssv(4, n,n, myAp, myAi, myAx, myb);

        System.out.println("Par superLU wrapped Klu solved time msec: "+ (System.nanoTime()-startTime)/1000000);
        //System.out.println("Peak memory:"+ Common.mempeak+" status "+Common.status);
        for (int i = n-10 ; i < n ; i++) {
            System.out.printf("x [%d] = %g    <->  my -> %g\n", i, b [i], myb[i]) ;
            //assertEquals(i + 1.0, b [i], DELTA) ;
        }
    }

    public static void main(String[] args) throws NoSuchFieldException, IllegalAccessException {
        //dump_matrix();
        //mtj_ccs_klu_test();
        //la4j_ccs_klu_test();
        superluwr_ccs_klu_test();
        System.out.println("\nstarting test with SuperLU wrapped everything\n");
        String property = System.getProperty("java.library.path");
        StringTokenizer parser = new StringTokenizer(property, ";");
        while (parser.hasMoreTokens()) {
            System.err.println(parser.nextToken());
        }
        //short_superLU_wrapped_ccs_test();
        short_superLU_wrapped_ccs_test_par();
        //dump_matrix();
        //la4j_ccs_klu_test_rajat();
        //superLU_par_wrapped_ccs_test_rajat();
    }
}
