/**
 * Created by mamaxropela on 01.10.14.
 */
public class Vector {
    private int[] vector;

    public Vector(int size){
        vector = new int[size];
    }

    public void Generate(int filledDigit){
        for (int i = 0; i < vector.length; i++)
            vector[i] = filledDigit;
    }
}
