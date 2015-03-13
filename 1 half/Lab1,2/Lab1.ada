---------------------------------------------------------------------------------
--            Laboratory Work #1    
--             Lab1.Ada.Packages                                                                      
--      Name      Pashchuk Eduard Fedorovich    
--      Group     IP-22                                                       
--      Date      09.09.2014 
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ))) 
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM) 
---------------------------------------------------------------------------------
with Data, Ada.Text_IO, Ada.Integer_Text_IO;  
use  Ada.Text_IO, Ada.Integer_Text_IO;


procedure Lab1 is
    package Int_Data is new Data(3);
    use Int_Data;
	X : Integer;
    A, B, C, M, Z : Vector;
    MA, MB, MC, MM, MZ, Y : Matrix;
    begin
        -- Input Data
        Input("A",a);
		input("B",B);
        Input("C",C);
        Input("M",M);
        Input("MA",MA);
        Input("MB",MB);
        Input("MC",MC);
        Input("MM",MM);
        Input("MZ",MZ);
        -- Calculate functions
        X := F1(A, B, C, MA, MZ);
        Y := F2(MA, MB);
        Z := F3(A, M, MC, MM);

        -- Output rezult
        Put("F1 = "); 
        Put(x); 
        New_Line; 
        New_Line; 

        Put_Line("F2 = ");  
        Output(y);
		new_line;
        Put("F3 = ");
        Output(z);

end Lab1;