program setstuff(input, output);

(*** Demonstrate the set datatype. ***)

type
   Digit = set of 0..9;

var
   odds, evens, stuff, morestuff: Digit;

begin
   odds := [1, 3, 5, 7, 9];
   evens := [0, 2, 4, 6, 8];
   stuff := [];
   morestuff := odds + [2];
   
   (* Check out of bounds integer (no compiler error) *)
   if 13 in morestuff then
      writeln("ERROR")
   else
      writeln("OK");
      
   (* Test set ranges *)
   stuff := [0..9];
   morestuff := [1..8];
   if morestuff <= stuff then
      writeln("OK")
   else
      writeln("ERROR");
   
   (* produce error: bad values *)
   (* stuff := [1, 42, 2]; *)
   (* stuff := [1, -42, 2]; *)
end.

