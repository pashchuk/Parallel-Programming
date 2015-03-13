with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Strings;
use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Strings;

generic
	Size:Integer;

package Data is
	--public member
	type Vector is private;
	type Matrix is private;
	function F1(A,B,C:Vector; MA,MZ:Matrix) return Integer;
	function F2(MA,MB:Matrix) return Matrix;
	function F3(A,M:Vector; MC,MM:Matrix) return Vector;

	procedure Input(Name: in string; A: out Vector);
	procedure Input(Name: in String; MA: out Matrix);
	procedure Output(A: in Vector);
	procedure Output(MA: in Matrix);

private
   --private member
   type Vector is array(1..Size) of Integer;
   type Matrix is array(1..Size) of Vector;

   function "*"(Left, Right : in Vector) return Integer;
   function "*"(Left: in Vector; Right: in Matrix) return Vector;
   function "*"(Left, Right : in Matrix) return Matrix;
   function "+"(Left, Right : in Vector) return Vector;
   function "+"(Left, Right : in Matrix) return Matrix;
   function Sort(Matr: in Matrix) return Matrix;
   function Sort(Vect: in Vector) return Vector;
   function Transpose(Matr: in Matrix) return Matrix;
   procedure Swap(A,B: in out Integer);

end Data;

package body Data is
    -- Define package implementation here
    function F1(A,B,C:Vector; MA,MZ:Matrix) return Integer is
	begin
		return (B*C)+(A*B)+(C*(B*(MA*MZ)));
	end f1;

	function F2(MA,MB:Matrix) return Matrix is
	begin
		return Sort(Transpose(MA)*MB);
	end f2;

	function F3(A,M:Vector; MC,MM:Matrix) return Vector is
	begin
		return Sort(A+M)*(MC*MM);
	end f3;

	procedure Input(name:in string; A : out Vector) is
	begin
		Put("Input Vector ");
		put(name);
		put_line(":");
        for i in 1..Size loop
            Get(A(i));
        end loop;
    end Input;
    
    procedure Output(A : in Vector) is
    begin
        for i in 1..Size loop
            Put(A(i));
            Put("   ");
        end loop;
        Put_Line("");
    end Output;
    
    procedure Input(Name: in String; MA : out Matrix) is
	begin
		Put("Input Matrix ");
		put(name);
		put_line(":");
        for i in 1..Size loop
            for j in 1..Size loop
                Get(MA(i)(j));
            end loop;
        end loop;
    end Input;
    
    procedure Output(MA : in Matrix) is
    begin
        for i in 1..Size loop
            for j in 1..Size loop
                Put(MA(i)(j), 9);
            end loop;
            Put_Line("");
        end loop;
        Put_Line("");
    end Output;

	function "*"(Left, Right : in Vector) return Integer is
    c : Integer := 0;
    begin
        for i in 1..Size loop
            c := c + Left(i) * Right(i);
        end loop;
        return c;
    end "*";
    
	function "*"(Left: in Vector; Right: in Matrix) return Vector is
    B : Vector;
    begin
        for i in 1..Size loop
            B(i) := 0;
            for j in 1..Size loop
                B(i) := B(i) + Left(j) * Right(j)(i);
            end loop;
        end loop;
        return B;
    end "*";
    
    function "*"(Left, Right : in Matrix) return Matrix is
    MC : Matrix;
    begin
        for i in 1..Size loop
            for j in 1..Size loop
            MC(i)(j) := 0;
                for k in 1..Size loop
                    MC(i)(j) := MC(i)(j) + Left(i)(k) * Right(k)(j);
                end loop;
            end loop;
        end loop;
        return MC;
    end "*";
    
    function "+"(Left, Right : in Matrix) return Matrix is
    MC : Matrix;
    begin
        for i in 1..Size loop
            for j in 1..Size loop
                MC(i)(j) := Left(i)(j) + Right(i)(j);
            end loop;
        end loop;
    return MC;
    end "+";
   
    function "+"(Left, Right : in Vector) return Vector is
    A : Vector;
    begin
        for i in 1..Size loop
            A(i) := Left(i) + Right(i);
        end loop;
        return A;
    end "+";

	function Sort(Vect: in Vector) return Vector is
		temp:Vector;
	begin
		temp := vect;			
		for i in 1..Size loop
			for j in 1..(Size - i) loop
				if temp(j) > Temp(j+1) then
					swap(Temp(j),Temp(j+1));
				end if;
			end loop;
		end loop;
		return Temp;
	end sort;
	
	function Sort(Matr: in Matrix) return Matrix is 
		V:Vector;
		M:Matrix;
	begin
		M:=Matr;
		for k in 1..Size loop
			v := M(k);
			for i in 1..Size loop
				for j in 1..(Size - i) loop
					if v(j) < v(j+1) then
						swap(v(j),v(j+1));
					end if;
				end loop;
			end loop;
			m(k) := v;
		end loop;
		return M;
	end sort;
			

    function Transpose(Matr: in Matrix) return Matrix is
		M:Matrix;
	begin
		for i in 1..Size loop
			for j in 1..Size loop
				m(j)(i) := matr(i)(j);
			end loop;
		end loop;
		return M;
	end Transpose;

	procedure Swap(A,B: in out Integer) is
		t:Integer;
	begin
		t := A;
		A := b;
		B := T;
	end Swap;
end data;
