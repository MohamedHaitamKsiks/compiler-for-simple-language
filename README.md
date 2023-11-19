# compiler-for-simple-language


Syntaxe:
```
    program ::= inst

    inst ::= insts | affec | variable | constant | whileBlock | ecrire | lire | si

    insts ::= { inst ; {inst ;} }

    affec ::= ID = expr
    variable ::= var ID { = expr }
    constant ::= const ID = expr

    si ::= if COND inst
    whileBlock ::= simo COND do inst

    ecrire  ::= print(expr {, expr})
    lire ::= input(ID)

    ...
    la suite est evidente

```

Exemple:

```
    var k = 1;
    var f = 1;
    var n = 1;

    input(n);
    simo(k <= n) {
        f = f * k;
        k = k + 1;
    };

    print(f);
```

Output:

```
> 5
120
```