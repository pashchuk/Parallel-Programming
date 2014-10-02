---------------------------------------------------------------------------------
--            Laboratory Work #2    
--             Lab2.Ada.Processes
--      Name      Pashchuk Eduard Fedorovich    
--      Group     IP-22                                                       
--      Date      28.09.2014 
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ))) 
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM) 
---------------------------------------------------------------------------------
with Data, Ada.Text_IO, Ada.Integer_Text_IO;  
use  Ada.Text_IO, Ada.Integer_Text_IO;


procedure Lab2 is
    package Int_Data is new Data(3);
    use Int_Data;

	protected type Mutex is
	    entry Seize;
		procedure Release;
	private
		Owned : Boolean := False;
	end Mutex;
	
	protected body Mutex is
		entry Seize when not Owned is
	    begin
			Owned := True;
	    end Seize;
	
	    procedure Release is
		begin
	    	Owned := False;
	    end Release;
	end Mutex;
	
	M : Mutex;
	
	   task T1 is
	      pragma Priority(3);
	      pragma Storage_Size(100000000);
	   end T1;
	   task body T1 is
	   	rezult : Integer;
	   	A, B, C : Vector;
		MA, MZ : Matrix;
	   begin
	      -- Input F1
	      M.Seize;
	      Put_Line("Task T1 started");
	      input("A",A);
		  input("B",B);
		  input("C",C);
		  input("MA",MA);
		  input("MZ",MZ);
	      M.Release;
	      delay (2.0);
	      Rezult := F1(A, B, C, MA, MZ);
	      M.Seize;
	      Put_Line("F1 = ");
		  new_line;
	      Put(rezult);
		  new_line;
	      Put_Line("Task T1 finished");
	      M.Release;
	   end T1;
	
	   task T2 is
	      pragma Priority(3);
	      pragma Storage_Size(100000000);
	   end T2;
	   task body T2 is
		MA, MB, Rezult : Matrix;
	   begin
	      -- Input F2
	      M.Seize;
	      Put_Line("Task T2 started");
	      input("MA",MA);
		  input("MB",MB);
	      M.Release;
	      delay (1.0);
	      Rezult := F2(MA, MB);
	      M.Seize;
	      Put_Line("F2 = ");
		  output(rezult);
	      Put_Line("Task T2 finished");
	      M.Release;
	   end T2;
	
	   task T3 is
	      pragma Priority(3);
	      pragma Storage_Size(100000000);
	   end T3;
	   task body T3 is
	   	Rezult, A, B : Vector;
		MC, MM : Matrix;
	   begin
	      -- Input F3
	      M.Seize;
	      Put_Line("Task T3 started");
		  input("A",A);
		  input("M",B);
		  input("MC",MC);
		  input("MM",MM);
	      M.Release;
	      delay(0.1);
	      Rezult := F3(A, B, MC, MM);
	      M.Seize;
	      Put_Line("F3 = ");
	      Output(Rezult);
	      Put_Line("Task T3 finished");
	      M.Release;
	   end T3;
	begin
	   null;
end Lab2;
