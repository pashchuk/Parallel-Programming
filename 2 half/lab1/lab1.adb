-------------------------------------
-- Lab1                            --
-- Pashchuk Eduard Fedorovich      --
-- Group IP-22                     --
-- Task MA = max(MO)*ME + MT*MK    --
-- Data : 25.02.2015               --
-------------------------------------
with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Synchronous_Task_Control;
use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Synchronous_Task_Control;

-- A=(B*MC)*(MO*MU)
procedure lab1 is
  -- dimension of vectors and matrixes
  N : Integer := 6;
  --the number of processors
  P: Integer := 2;
  H: Integer := N/P;
  -- Users types
  type Vector is Array (1..N) of Integer;
   type Matrix is Array (1..N) of Vector;
   type Matrix_Ptr is access Matrix;
   MA : Matrix_Ptr := new Matrix;
   MO : Matrix_Ptr := new Matrix;
   MK : Matrix_Ptr := new Matrix;
   ME : Matrix_Ptr := new Matrix;
   MT : Matrix_Ptr := new Matrix;
  --MA, MO, MK, ME, MT, MX, MY: Matrix;
   S0, S11, S12, S21, S22, S23 : Suspension_Object;
  m: Integer;
  -- Input vector's values
  procedure Vector_Input(A: in out Vector) is
  begin
     for i in 1..N loop
        A(i) := 1;
     end loop;
  end Vector_Input;
  -- Output vector
  procedure Vector_Output(A: in Vector) is
  begin
     for i in 1..N loop
        Put( A(i));
     end loop;
     New_Line;
  end Vector_Output;
  -- Input matrix's values
  procedure Matrix_Input(MA: in out Matrix_Ptr) is
  begin
     for i in 1..N loop
        for j in 1..N loop
           MA(i)(j):=1;
        end loop;
     end loop;
  end Matrix_Input;
  -- Output matrix
  procedure Matrix_Output(MA: in Matrix_Ptr) is
  begin
     for i in 1..N loop
        for j in 1..N loop
           Put(MA(i)(j));
        end loop;
        New_Line;
     end loop;
   end Matrix_Output;

  procedure Matrix_SetZero(MA: in out Matrix_Ptr) is
  begin
     for i in 1..N loop
        for j in 1..N loop
           MA(i)(j):=0;
        end loop;
     end loop;
  end Matrix_SetZero;
   -- tasks
   task T1 is
     pragma CPU(1);
  end T1;
  task body T1 is
     MK1: Matrix_Ptr := new Matrix;
     m1, x1, temp: Integer := 0;
   begin
      Put_Line("T1 started");
      -- Entering MO, MK
      Matrix_Input(MO);
      Matrix_Input(MK);
     --give a signal11
      Set_True(S11);
     --wait for signal21
      Suspend_Until_True(S21);
      --coping MK;
      Suspend_Until_True(S0);
      MK1 := MK;
      Set_True(S0);
     x1 := -9999999;
     --find max value in MOh
        for i in 1..H loop
           for j in 1..N loop
              if MO(i)(j) > x1 then
                 x1 := MO(i)(j);
              end if;
           end loop;
      end loop;
      Suspend_Until_True(S0);
      if x1 > m then
         m := x1;
      end if;
      Set_True(S0);
      --give signal12
      Set_True(S12);
      --Wait for signal22
      Suspend_Until_True(S22);
      --coping m
      Suspend_Until_True(S0);
      m1 := m;
      Set_True(S0);
      -- calculate MA
      for i in 1..H loop
        for j in 1..N loop
           for k in 1..N loop
               temp := temp + MT(i)(k)*MK1(k)(j);
            end loop;
            MA(i)(j):=m1*ME(i)(j)+temp;
            temp := 0;
        end loop;
      end loop;
      --wait for signal23
      Suspend_Until_True(S23);
      Put_Line("T1 finished");
      Put_Line("MA = ");
      Matrix_Output(MA);
   end T1;
  task T2 is
     pragma CPU(2);
  end T2;
  task body T2 is
     MK2: Matrix_Ptr := new Matrix;
     m2, x2, temp: Integer := 0;
   begin
      Put_Line("T2 started");
      -- Entering ME, MT
      Matrix_Input(ME);
      Matrix_Input(MT);
     --give a signal21
      Set_True(S21);
     --wait for signal11
      Suspend_Until_True(S11);
      --coping MK;
      Suspend_Until_True(S0);
      MK2 := MK;
      Set_True(S0);
     x2 := -9999999;
     --find max value in MOh
        for i in H..N loop
           for j in 1..N loop
              if MO(i)(j) > x2 then
                 x2 := MO(i)(j);
              end if;
           end loop;
      end loop;
      Suspend_Until_True(S0);
      if x2 > m then
         m := x2;
      end if;
      Set_True(S0);
      Set_True(S22);
      Suspend_Until_True(S12);
      --coping m
      Suspend_Until_True(S0);
      m2 := m;
      Set_True(S0);
      --calculate MA
      for i in H..N loop
        for j in 1..N loop
           for k in 1..N loop
               temp := temp + MT(i)(k)*MK2(k)(j);
            end loop;
            MA(i)(j):=m2*ME(i)(j)+temp;
            temp := 0;
        end loop;
      end loop;
      --give a signal23
      Set_True(S23);
      Put_Line("T2 finished");
   end T2;
begin
   m := -9999999;
   Set_True(S0);
end lab1;
