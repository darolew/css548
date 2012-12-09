program arrays(input,output);

(*** Demonstrate array usage. ***)

type
      array1D = array [1..5] of integer;
      array3D = array [1..5,2..3,0..2] of integer;

var
      a: array3D;
      b: array [0..3] of array1D;
      i,j,k: integer;

begin
   for i := 0 to 3 do begin
      for j := 1 to 5 do
         write(b[i,j], " ");
      writeln;
   end;
end.

