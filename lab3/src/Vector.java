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

    public static int Multiply(Vector v1, Vector v2){
        int rezult = 0;
        for (int i = 0; i < v1.getSize(); i++)
            rezult += v1.getElement(i)*v2.getElement(i);
        return rezult;
    }

    public static Vector Plus(Vector v1, Vector v2){
        Vector rezult = new Vector(v1.getSize());
        for (int i = 0; i < v1.getSize(); i++)
            rezult.setElement(i,v1.getElement(i) + v2.getElement(i));
        return rezult;
    }

    public int getSize(){
        return vector.length;
    }
}
