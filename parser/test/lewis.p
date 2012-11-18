program lewis(input,output);

type
    A = integer;
    B = real;
    C = char;
    D = boolean;
    ARRAY1 = array [1..5] of integer;
    ARRAY2 = array [1..9] of integer;
    ARRAY3 = array [5..9] of integer;
    APTR = ^A;
    cellPtr = ^cell;
    cell = record
        id: integer;
        foo: real; 
        next: cellPtr
    end;

var
    va: A;
    vb: B;
    vc: C;
    vd: D;    
begin
    va := 2;
end.
