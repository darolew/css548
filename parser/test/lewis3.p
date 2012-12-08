program lewis3(input,output);

type
  integerArray = array [1..6] of integer;
  cellPtr = ^cell;
  cell = record
     id: integer;
     info: integerArray;
     next: cellPtr
  end;

function average(newrec: cellPtr): integer;   
var
    i, sum : integer;
begin
    sum := 0;
    for i := 1 to 5 do
       sum:=sum + newrec^.info[i];
    average:=sum div 5;
end;

begin
end.
