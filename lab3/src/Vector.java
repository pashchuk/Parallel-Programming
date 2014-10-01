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
    public int getElement(int index){
        return vector[index];
    }
    public void setElement(int index, int value){
        vector[index] = value;
    }
    public int getSize(){
        return vector.length;
    }
}
