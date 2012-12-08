program linkedlist(input,output);

type
  integerArray = array [1..6, 4..7] of integer;

  cellPtr = ^cell;
  cell = record
     info: integerArray;
     next: cellPtr
  end;

var
    newrec: ^cell;
    x: integer;
    i: integer;
  
begin
  new(newrec);
  x := newrec^.info[i, 2+1];
  
end.