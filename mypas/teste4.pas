program teste (input, output);
	{ lexical level 1 }
	var x,y: long;
	procedure A(var z: integer; w: integer);
		{ lexical level 2 }
		var t: integer;
		begin
			t := z + w * x * y
		end;
	{ lexical level 1 }
	function factorial(n: integer) : integer;
		{ lexical level 2 }
		begin
			if n > 2 then
				factorial := factorial(n-1) * n
				{ in C: return factorial := factorial(n-1) * n; }
			else
				factorial := 1
				{ in C: return 1; }
		end;
begin
	y:= x + y
end.
