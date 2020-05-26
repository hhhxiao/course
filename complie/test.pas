begin
 integer x;
  integer function f(n);
    begin
      integer n;
      if n<=0 then f:=1
      elso f:=n*f(n-1)
    end;
  read (m);
  k:= f(m);
  write(k)
end
