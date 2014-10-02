import java.util.Arrays;
import java.util.Collections;

/**
 * Created by mamaxropela on 01.10.14.
 */
public class Matrix {
    private int[][] matrix;

    public Matrix(int size){
        matrix = new int[size][size];
    }

    public void Generate(int filledDigit){
        for (int i = 0; i < matrix.length; i++)
            for(int j = 0; j < matrix.length; j++)
                matrix[i][j] = filledDigit;
    }

    public static Matrix Multiply(Matrix m1, Matrix m2){
        Matrix rezult = new Matrix(m1.getSize());
        int tempValue = 0;
        for (int i = 0; i < rezult.getSize(); i++)
            for (int j = 0; j < rezult.getSize(); j++) {
                tempValue = 0;
                for (int k = 0; k < rezult.getSize(); k++)
                    tempValue += m1.getElement(j,k) * m2.getElement(k,j);
                rezult.setElement(i,j,tempValue);
            }
        return rezult;
    }

    public static Vector Multiply(Matrix matrix, Vector vector){
        Vector rezult = new Vector(vector.getSize());
        int tempValue = 0;
        for(int i = 0; i < rezult.getSize(); i++){
            tempValue = 0;
            for (int j = 0; j < rezult.getSize(); j++)
                tempValue += vector.getElement(i) * matrix.getElement(j,i);
            rezult.setElement(i,tempValue);
        }
        return rezult;
    }

    public static Matrix Plus(Matrix m1, Matrix m2){
        Matrix rezult = new Matrix(m1.getSize());
        for (int i = 0; i < m1.getSize(); i++)
            for (int j = 0; j < m1.getSize(); j++)
                rezult.setElement(i,j,m1.getElement(i,j) + m2.getElement(i,j));
        return rezult;
    }

    public Matrix Sort(){
        Integer[] tempArr = new Integer[matrix.length];
        for (int i = 0; i < matrix.length; i++){
            for (int j = 0; j < matrix.length; j++)
                tempArr[j] = matrix[i][j];
            Arrays.sort(tempArr,Collections.reverseOrder());
            for(int j = 0; j < matrix.length; j++)
                matrix[i][j] = tempArr[j];
        }
        return this;
    }

    public Matrix Transpose(){
        Matrix rezult = new Matrix(matrix.length);
        for (int i = 0; i < matrix.length; i++)
            for(int j = 0; j < matrix.length; j++)
                rezult.setElement(i,j,matrix[j][i]);
        return rezult;
    }

    public int getSize(){
        return matrix.length;
    }

    public int getElement(int line, int column){
        return matrix[line][column];
    }
    public void setElement(int line, int column, int value){
        matrix[line][column] = value;
    }

}
