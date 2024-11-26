program ExemploBasico (input, output);
var x, y, soma: integer;

function Dobro(valor: integer): integer;
begin
    Dobro := valor * 2;
end;

procedure Incrementa(var z: integer);
begin
    z := z + 1;
end;

begin
    { Inicialização das variáveis }
    x := 5;
    y := 10;
    soma := 0;

    { Estrutura condicional IF }
    if x < y then
        x := x + y
    else
        y := y - x;

    { Estrutura WHILE }
    while x > 0 do
    begin
        soma := soma + x;
        x := x - 1;
    end;

    { Estrutura REPEAT }
    repeat
        y := y - 1;
    until y = 0;

    { Uso da função e do procedimento }
    soma := Dobro(soma);
    Incrementa(soma);
end.