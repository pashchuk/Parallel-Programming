/**
 * Created by mamaxropela on 01.10.14.
 */
public class Matrix {
    private int size;
    private int[][] matrix;

    public Matrix(int size){
        this.size = size;
        matrix = new int[size][size];
    }
    public void Generate(int filledDigit){
        for (int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                matrix[i][j] = filledDigit;
    }


}
