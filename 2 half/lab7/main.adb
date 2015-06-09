with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Calendar;
    use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Calendar;
    
    ---------------------------------
    -- PP2 Lab 7                    -
    -- Pashchuk Eduard              -
    -- IP-22                        -
    -- 28.05.15                     -
    -- A = alpha*B + C*(MO*MK)      -
    ---------------------------------
    procedure Lab7 is
       N: Integer := 8;
       P: Integer := 8;
       H: Integer := N / P;
    
       type Vector is array(Integer range<>) of Integer;
       subtype Vector_H is Vector(1..H);
       subtype Vector_2H is Vector(1..2*H);
       subtype Vector_3H is Vector(1..3*H);
       subtype Vector_4H is Vector(1..4*H);
       subtype Vector_5H is Vector(1..5*H);
	   subtype Vector_6H is Vector(1..6*H);
	   subtype Vector_7H is Vector(1..7*H);
       subtype Vector_N is Vector(1..N);
    
       type Matrix is array(Integer range<>) of Vector_N;
       subtype Matrix_H is Matrix(1..H);
       subtype Matrix_2H is Matrix(1..2*H);
       subtype Matrix_3H is Matrix(1..3*H);
       subtype Matrix_4H is Matrix(1..4*H);
       subtype Matrix_5H is Matrix(1..5*H);
	   subtype Matrix_6H is Matrix(1..6*H);
	   subtype Matrix_7H is Matrix(1..7*H);
       subtype Matrix_N is Matrix(1..N);
    
       ---------------------------------------------------------------
       -- Input vector
       procedure VectorInput(M: out Vector_N) is
       begin
          for i in 1..N loop
             M(i) := 1;
          end loop;
       end VectorInput;
    
       -- Output vector
       procedure VectorOutput(M: in Vector_N) is
       begin
          if ( N < 20 ) then
             for i in 1..N loop
                Put(M(i));
             end loop;
             New_Line;
          end if;
       end VectorOutput;
    
       -- Input matrix
       procedure MatrixInput(MA: out Matrix_N) is
       begin
          for i in 1..N loop
             for j in 1..N loop
                MA(i)(j) := 1;
             end loop;
          end loop;
       end MatrixInput;
    
       -- ???????? ???? ?????
       function Max(x, y: in Integer) return Integer is
       begin
          if ( x > y ) then
             return x;
          end if;
          return y;
       end;
    
       -- ????? ????????? ? ???????
       function VectorMax(M: in Vector_H) return Integer is
          a: Integer;
       begin
          a := Integer'First;
          for i in 1..H loop
             a := Max(a, M(i));
          end loop;
          return a;
       end VectorMax;
    
       -- Calculate A
       procedure Calculate(alpha: Integer; B: in Vector_H; C: in Vector_N; MO: in Matrix_N; MK: in Matrix_H; A: out Vector_H) is
          t: Integer;
       begin
          for i in 1..H loop
             A(i) := alpha * B(i);
             for j in 1..N loop
                t := 0;
                for k in 1..N loop
                   t := t + MO(j)(k) * MK(I)(K);
                end loop;
                A(i) := A(i) + C(j) * t;
             end loop;
          end loop;
       end Calculate;
    
       ---------------------------------------------------------------
       -- specification of type for task T_i
      task type Task_I(Tid: Integer) is
         entry DataInput1(C: in Vector; MO: in Matrix; alph: in Integer);
         entry DataInput2(B: in Vector; MK: in Matrix);
   
         entry Result_A(A: in Vector);
      end Task_I;
      ---------------------------------------------------------------
      -- pointer types for task
      type Task_I_Ptr is access Task_I;
   
      -- aray type of pointer
      type Tasks_Array is array (1..P) of Task_I_Ptr;
   
      tasksArray: Tasks_Array;
   
      ---------------------------------------------------------------
      --  Implementation task T_I
      task body Task_I is
         beginning: Integer := (Tid - 1) * H + 1;
         ending: Integer := Tid  * H;
   
         B7h, A7H: Vector_7H;
         B6h, A6H: Vector_6H;
		 B5h, A5H: Vector_5H;
		 B4h, A4H: Vector_4H;
		 B3h, a3H: Vector_3H;
		 B2h, A2H: Vector_2H;
		 Bh, Ch, Ah: Vector_H;
         An, Bn, cn: Vector_N;
		 MK7h: Matrix_7H;
		 MK6h: Matrix_6H;
		 MK5h: Matrix_5H;
		 MK4h: Matrix_4H;
		 MK3h: Matrix_3H;
		 MK2h: Matrix_2H;
         MOh, MKh: Matrix_H;
       
         MOn, MKn: Matrix_N;
         alpha: Integer;
      begin
         Put_Line("task" & Integer'Image(Tid) & " started!!!");
   
         if Tid = 1 then
            VectorInput(Cn);
			MatrixInput(MOn);
            Alpha := 1;
                    Ch := Cn(1..H);
					MOh := MOn(1..H);
		 else accept DataInput1(C: in Vector; MO: in Matrix; alph: in Integer) do
               Cn := C;
               MOn := MO;
               alpha := Alph;
            end DataInput1;
 		end if;

--------------------------------------------------------------------------
         if Tid = 1 then
            tasksArray(2).DataInput1(Cn, MOn, Alpha);
         end if;
         if Tid = 2 then
            TasksArray(3).DataInput1(cn, MOn, Alpha);
			TasksArray(4).DataInput1(Cn, MOn, Alpha);
			end if; 
         if Tid = 3 then
            TasksArray(7).DataInput1(Cn, MOn, Alpha);
         end if;
         if Tid = 4 then
            TasksArray(5).DataInput1(Cn, MOn, Alpha);
         end if;
		 if Tid = 5 then
            TasksArray(6).DataInput1(Cn, MOn, Alpha);
         end if;
		 if Tid = 7 then
            TasksArray(8).DataInput1(Cn, MOn, Alpha);
         end if;

---------------------------------------------------------------------------

         if Tid = 1 then
            accept DataInput2(B: in Vector; MK: in Matrix) do
               Bh := B;
               MKh := MK;
            end DataInput2;
         elsif Tid = 2 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               B5h := B;
               MK5h := MK;
            end DataInput2;
                    Bh := B5h(H+1..2*H);
                    MKh := MK5h(H+1..2*H);
					b3h := B5h(2*h+1..5*H);
					MK3h := MK5h(2*h+1..5*H);
		  elsif Tid = 3 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               B6h := B;
               MK6h := MK;
            end DataInput2;
                    Bh := B6h(2*H+1..3*H);
                    MKh := MK6h(2*H+1..3*H);
					b5h(1..2*h) := B6h(1..2*H);
					b5h(2*h+1..5*h) := B6h(3*h+1..6*H);
					MK5h(1..2*h) := MK6h(1..2*H);
					MK5h(2*h+1..5*h) := MK6h(3*h+1..6*H);
		 elsif Tid = 4 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               B3h := B;
               MK3h := MK;
            end DataInput2;
                    Bh := B3h(1..H);
                    MKh := MK3h(1..H);
		  elsif Tid = 5 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               B2h := B;
               MK2h := MK;
            end DataInput2;
					Bh := B2h(1..H);
                    MKh := MK2h(1..H);
          elsif Tid = 6 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               Bh := B;
               MKh := MK;
            end DataInput2;
          elsif Tid = 7 Then 
            accept DataInput2(B: in Vector; MK: in Matrix) do
               B7h := B;
               MK7h := MK;
            end DataInput2;
					Bh := B7h(6*H+1..7*H);
                    MKh := MK7h(6*H+1..7*H);
		  elsif Tid = 8 Then
		  	VectorInput(Bn);
			MatrixInput(MKn);
                    Bh := Bn(7*h+1..8*H);
					MKh := MKn(7*h+1..8*H);
          end if;


         if Tid = 2 then
		 	TasksArray(1).DataInput2(B5h(1..H), MK5h(1..H));
			TasksArray(4).DataInput2(B3h, MK3h);
			end if; 
         if Tid = 3 then
            TasksArray(2).DataInput2(B5h, MK5h);
         end if;
         if Tid = 4 then
            TasksArray(5).DataInput2(B3h(h+1..3*H), MK3h(h+1..3*H));
         end if;
		 if Tid = 5 then
            TasksArray(6).DataInput2(B2h(h+1..2*H), MK2h(h+1..2*H));
         end if;
		 if Tid = 7 then
            TasksArray(3).DataInput2(B7h(1..6*H), MK7h(1..6*H));
         end if;
		 if Tid = 8 then
            TasksArray(7).DataInput2(Bn(1..7*H), MKn(1..7*H));
         end if;
		 -----------------------------------------------------------

         Calculate(alpha, Bh, Cn, MOn, mKh, Ah);
   
         if Tid = 1 then
            tasksArray(2).Result_A(Ah);
         elsif Tid = 2 then
		 	A5h(H+1..2*H) := Ah;
			accept Result_A(A: in Vector) do
               A5h(1..H) := A; 
            end Result_A;
			accept Result_A(A: in Vector) do
               A5h(2*h+1..5*H) := A; 
            end Result_A;
            tasksArray(3).Result_A(A5h);
         elsif Tid = 3 then
            A6h(2*h+1..3*h) := Ah;
            accept Result_A(A: in Vector) do
               A6h(1..2*H) := A(1..2*H);
  			   A6h(3*H+1..6*H) := A(2*H+1..5*H);
            end Result_A;
            tasksArray(7).Result_A(A6h);
		 elsif Tid = 4 then
            A3h(1..H) := Ah;
            accept Result_A(A: in Vector) do
               A3h(H+1..3*H) := A;
            end Result_A;
            tasksArray(2).Result_A(A3h);
		 elsif Tid = 5 then
            A2h(1..H) := Ah;
            accept Result_A(A: in Vector) do
               A2h(H+1..2*H) := A;
            end Result_A;
            tasksArray(4).Result_A(A2h);
		elsif Tid = 6 then        
            tasksArray(5).Result_A(Ah);
		elsif Tid = 7 then
            A7h(6*h+1..7*h) := Ah;
            accept Result_A(A: in Vector) do
               A7h(1..6*H) := A;
            end Result_A;
            tasksArray(8).Result_A(A7h);
		elsif Tid = 8 then
            An(7*h+1..8*h) := Ah;
            accept Result_A(A: in Vector) do
               An(1..7*H) := A;
            end Result_A;
            VectorOutput(An);
		end if;
   
         Put_Line("task" & Integer'Image(Tid) & " finished!!!");
      end Task_I;
   
   ---------------------------------------------------------------
   begin
      for i in 1..P loop
         if i > 1 then
            tasksArray(i) := new Task_I(i);
         end if;
      end loop;
   tasksArray(1) := new Task_I(1);
   end Lab7;