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


}
