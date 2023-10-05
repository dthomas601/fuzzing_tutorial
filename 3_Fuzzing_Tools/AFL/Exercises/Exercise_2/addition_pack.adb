package body addition_pack is
	function Add_Func(A,B : Integer) return Integer is
	begin
		return A+B;
	end Add_Func;
end addition_pack;



--procedure Add_Values is
--   N1 : Integer;
--   N2 : Integer;
--   SUM: Integer;
--begin
   --  Put a String
--   Put ("Enter 1 an integer value: ");
   --  Read in an integer value
--   Get (N1);
   
   --  Put a String
--   Put ("Enter 2 an integer value: ");
   --  Read in an integer value
--   Get (N2);

--   SUM := N1+N2;

   --  Put an Integer
--   Put(SUM);
--   Put_Line (" is the sum. ");
--end Add_Values;
