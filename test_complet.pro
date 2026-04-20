BeginProject monprog;

Setup :
    %% ---- Declarations de variables simples ----
    define x : integer;
    define y : integer;
    define z : integer;
    define res : float;
    define flag : integer;

    %% ---- Declaration avec initialisation ----
    define compteur : integer = 0;
    define moyenne : float = 0.0;

    %% ---- Declaration de plusieurs variables en une ligne ----
    define a | b | c : integer;

    %% ---- Declaration d'une constante ----
    const MAX : integer = 10;
    const PI : float = 3.14;

    %% ---- Declaration d'un tableau ----
    define Tab : [integer ; 5] ;

    %% ---- Variable de boucle ----
    define i : integer;
    define j : integer;

Run :
{
    //* ================================================
        TEST 1 : Affectations simples
       ================================================ *//
    x <- 5;
    y <- 3;
    z <- 0;

    //* ================================================
        TEST 2 : Operations arithmetiques
       ================================================ *//
    z <- x + y;
    z <- x - y;
    z <- x * y;
    res <- x / y;
    z <- (x + y) * (x - y);

    //* ================================================
        TEST 3 : IF sans ELSE
       ================================================ *//
    if (x > y) then:
    {
        z <- 1;
    } endIf;

    //* ================================================
        TEST 4 : IF avec ELSE
       ================================================ *//
    if (x > y) then:
    {
        flag <- 1;
    } else {
        flag <- 0;
    } endIf;

    //* ================================================
        TEST 5 : IF imbrique
       ================================================ *//
    if (x > 0) then:
    {
        if (y > 0) then:
        {
            z <- x + y;
        } else {
            z <- x - y;
        } endIf;
    } else {
        z <- 0;
    } endIf;

    //* ================================================
        TEST 6 : Operateurs de comparaison
       ================================================ *//
    if (x == y) then:
    {
        flag <- 1;
    } endIf;

    if (x != y) then:
    {
        flag <- 2;
    } endIf;

    if (x >= y) then:
    {
        flag <- 3;
    } endIf;

    if (x <= y) then:
    {
        flag <- 4;
    } endIf;

    //* ================================================
        TEST 7 : Operateurs logiques AND / OR / NON
       ================================================ *//
    if (x > 0 AND y > 0) then:
    {
        z <- 1;
    } endIf;

    if (x > 0 OR y > 0) then:
    {
        z <- 2;
    } endIf;

    if (NON(flag == 0)) then:
    {
        z <- 3;
    } endIf;

    //* ================================================
        TEST 8 : Boucle WHILE simple
       ================================================ *//
    x <- 3;
    loop while (x > 0) {
        x <- x - 1;
    } endloop;

    //* ================================================
        TEST 9 : Boucle WHILE avec IF a l interieur
       ================================================ *//
    x <- 5;
    loop while (x > 0) {
        if (x == 3) then:
        {
            flag <- 99;
        } endIf;
        x <- x - 1;
    } endloop;

    //* ================================================
        TEST 10 : Boucle WHILE imbriquee
       ================================================ *//
    i <- 3;
    loop while (i > 0) {
        j <- 2;
        loop while (j > 0) {
            z <- i + j;
            j <- j - 1;
        } endloop;
        i <- i - 1;
    } endloop;

    //* ================================================
        TEST 11 : Boucle FOR simple
       ================================================ *//
    z <- 0;
    for i in 1 to 5 {
        z <- z + i;
    } endfor;

    //* ================================================
        TEST 12 : Boucle FOR avec IF a l interieur
       ================================================ *//
    for i in 1 to 5 {
        if (i == 3) then:
        {
            flag <- 33;
        } endIf;
        z <- z + i;
    } endfor;

    //* ================================================
        TEST 13 : Boucle FOR imbriquee
       ================================================ *//
    z <- 0;
    for i in 1 to 3 {
        for j in 1 to 3 {
            z <- z + i + j;
        } endfor;
    } endfor;

    //* ================================================
        TEST 14 : Tableaux - ecriture et lecture
       ================================================ *//
    Tab[0] <- 10;
    Tab[1] <- 20;
    Tab[2] <- 30;
    x <- Tab[0] + Tab[1];

    //* ================================================
        TEST 15 : Remplir un tableau avec FOR
       ================================================ *//
    for i in 0 to 4 {
        Tab[i] <- i + 1;
    } endfor;

    //* ================================================
        TEST 16 : Input / Output
       ================================================ *//
    in(x);
    out("La valeur de x est : ", x);
    out("La valeur de z est : ", z);
    out("Fin du programme");
}

EndProject ;
