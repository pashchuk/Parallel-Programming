---------------------------------
-- PP2 Lab 6                      -
-- Pashchuk Eduard                -
-- IP-22                          -
-- 20.05.15                       -
-- MA = MB*(MC*MK) + (B*R)*MU     -
---------------------------------

with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Synchronous_Task_Control;
use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Synchronous_Task_Control;

procedure lab6 is
   N : Integer := 16;
   P: Integer := 8;
   H: Integer := N/P;

   -- Users types
   type Vector is Array (1..N) of Integer;
   type Matrix is Array (1..N) of Vector;
   B, R : Vector;
   MA, MB, MC, MK, MU : Matrix;

   -------------- INPUT OUTPUT --------------
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
   procedure Matrix_Input(MA: in out Matrix) is
   begin
      for i in 1..N loop
         for j in 1..N loop
            MA(i)(j):=1;
         end loop;
      end loop;
   end Matrix_Input;

   -- Output matrix
   procedure Matrix_Output(MA: in Matrix) is
   begin
      for i in 1..N loop
         for j in 1..N loop
            Put(MA(i)(j));
         end loop;
         New_Line;
      end loop;
   end Matrix_Output;

   -- Copy vector's values
   procedure Vector_Copy(A: in Vector; M: out Vector) is
   begin
      for i in 1..N loop
        M(i):=A(i);
      end loop;
   end Vector_Copy;

   -- Copy matrix's values
   procedure Matrix_Copy(MA: in Matrix; MM: out Matrix) is
   begin
      for i in 1..N loop
         for j in 1..N loop
            MM(i)(j):=MA(i)(j);
         end loop;
      end loop;
   end Matrix_Copy;

   ------------------ ZVI   ------------------
   protected ResourcesMonitor is
      function getMC return Matrix;
      procedure WriteMC (MA: in Matrix);
	  function getMB return Matrix;
      procedure WriteMB (MA: in Matrix);
      function Getd return Integer;
      procedure Writed (x: in Integer);
   private
      d : Integer;
      MB, MC : Matrix;
   end ResourcesMonitor;

   protected body ResourcesMonitor is
      function Getd return Integer is
      begin
         return D;
      end Getd;
      procedure Writed (x: in Integer) is
      begin
         D := x;
      end Writed;
      function getMC return Matrix is
      begin
         return MC;
      end getMC;
      procedure WriteMC (MA: in Matrix) is
      begin
         Matrix_Copy(MA, MC);
      end WriteMC;
      function getMB return Matrix is
      begin
         return MB;
      end getMB;
      procedure WriteMB (MA: in Matrix) is
      begin
         Matrix_Copy(MA, MB);
      end WriteMB;
   end ResourcesMonitor;

   ------------------- SN ------------------
   protected SynchronizingTask is

      entry WaitInput;
      entry WaitCalculation1;
      entry WaitCalculation2;
      procedure InputSignal;
      procedure Calculate1Signal;
      procedure Calculate2Signal;
   private
      flaginput : Integer := 0;
      flagcount1 : Integer := 0;
      flagcount2 : Integer := 0;
      P : Integer := 8;
   end SynchronizingTask;

   protected body SynchronizingTask is

      entry WaitInput
        when flaginput = 1 is
      begin
         null;
      end WaitInput;

      entry WaitCalculation1
        when flagcount1 = P is
      begin
         null;
      end WaitCalculation1;

      entry WaitCalculation2
        when flagcount2 = P is
      begin
         null;
      end WaitCalculation2;

      procedure InputSignal is
      begin
         flaginput := 1;
      end InputSignal;

      procedure Calculate1Signal is
      begin
         flagcount1 := flagcount1 + 1;
      end Calculate1Signal;

      procedure Calculate2Signal is
      begin
         flagcount2 := flagcount2 + 1;
      end Calculate2Signal;
   end SynchronizingTask;

   ----------------- COUNTING -----------------
   function calculation1(taskNum: in Integer; E: in Vector) return Integer is
      start : Integer := (taskNum-1)*H+1;
      to : Integer;
      --x : Integer := E(start);
   begin
      a := E(start);
      if taskNum=P then
         to := N;
      else
         to:= taskNum*H;
      end if;

      -- B*C;
      for i in start..to loop
         if a < E(i) then
            a := E(i);
         end if;
      end loop;

      return a;
   end calculation1;

   procedure calculation2(taskNum, a, alpha: in Integer; MB, MC, MM: in Matrix; MA : in out Matrix) is
      start : Integer := (taskNum - 1) * H+1;
      to : Integer;
      sum : Integer := 0;
   begin
      if taskNum=P then
         to :=N;
      else
         to:=taskNum*H;
      end if;

      for i in 1..N loop
         for j in start..to loop
            sum :=0;
            for k in 1..N loop
               sum := sum + MB(i)(k) * MC(k)(j);
            end loop;
            MA(i)(j) := a * MM(i)(j) + sum * alpha;
         end loop;
      end loop;

   end calculation2;

   ------------------ TASKS ------------------
   procedure overallTaskPart (taskNum:Integer)is
      ai, alphai: Integer;
      MBi: Matrix;
   begin
      SynchronizingTask.WaitInput;

      ai:= calculation1(taskNum, E);
      ResourcesMonitor.writea(ai);

      SynchronizingTask.Calculate1Signal;
      SynchronizingTask.WaitCalculation1;

      ai := ResourcesMonitor.getaCopy;
      alphai := ResourcesMonitor.getAlphaCopy;
      MBi := ResourcesMonitor.getMBCopy;

      calculation2(taskNum, ai, alphai, MB, MC, MM, MA);

      SynchronizingTask.Calculate2Signal;
   end overallTaskPart;

   ------------------ T1 ------------------
   task T1 is
   end T1;
   task body T1 is
      taskNum:Integer:=1;
   begin
      Put_Line("T1 started");

      Matrix_Input(MB);
      MutualExclusionTask.writeMB(MB);

      SynchronizingTask.InputSignal;

      overallTaskPart(taskNum);

      SynchronizingTask.WaitCalculation2;
      -- Output vector
      Matrix_Output(MA);
      Put_Line("T1 finished");
   end T1;

------------------ T3 ------------------
   task T3 is
   end T3;
   task body T3 is
      taskNum:Integer:=3;
   begin
      Put_Line("T3 started");

      Matrix_Input(MC);
      alpha := 1;
      MutualExclusionTask.writeAlpha(alpha);
      --give a signal of inputing
      SynchronizingTask.InputSignal;
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T3 finished");
   end T3;

   ------------------ T6 ------------------
   task T6 is
   end T6;
   task body T6 is
      taskNum:Integer:=6;
   begin
      Put_Line("T6 started");
      Vector_Input(E);
      Matrix_Input(MM);
      --give a signal of inputing
      SynchronizingTask.InputSignal;
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T6 finished");
   end T6;

   ------------------ T2 ------------------
   task T2 is
   end T2;
   task body T2 is
      taskNum:Integer:=2;
   begin
      Put_Line("T2 started");
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T2 finished");
   end T2;

   ------------------ T4 ------------------
   task T4 is
   end T4;
   task body T4 is
      taskNum:Integer:=4;
   begin
      Put_Line("T4 started");
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T4 finished");
   end T4;

   ------------------ T5 ------------------
   task T5 is
   end T5;
   task body T5 is
      taskNum:Integer:=5;
   begin
      Put_Line("T5 started");
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T5 finished");
   end T5;

   task T7 is
   end T7;
   task body T7 is
      taskNum:Integer:=7;
   begin
      Put_Line("T7 started");
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T7 finished");
   end T7;

   task T8 is
   end T8;
   task body T8 is
      taskNum:Integer:=8;
   begin
      Put_Line("T8 started");
      -- Counting
      overallTaskPart(taskNum);
      Put_Line("T8 finished");
   end T8;

begin
   null;
end lab6;
