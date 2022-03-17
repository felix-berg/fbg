# Bresenham circle algorithm
<a href="https://en.wikipedia.org/wiki/Midpoint_circle_algorithm">Wikipedia</a>
Algoritme, som udregner pixelværdier for den oktant i cirklen, der går fra 0 grader til 45 grader, og transformerer den til de otte andre positioner.

Center i (0, 0)
Start ved (r, 0)

Bevæg mod position y. Bevæg i negativ x.
For centerpunktet er 0, 0, holder ligningen for hvert punkt $n$:
$$
	r^2 = x_n^2 + y_n^2 \\
	x_n^2 = r^2 - y_n^2
$$
..., hvor $x_0 = r$ og $y_0 = 0$.
Da y for hvert punkt ændres med 1, fås:
$$
	y_n = y_{n - 1} + 1
$$
..., hvilket indsættes i formlen:
$$
\begin{align*}
	x_n^2&=r^2 - (y_{n - 1} + 1)^2 \\
	x_n^2&= r^2 - y_{n - 1}^2  - 1 - 2\cdot y_{n - 1}
\end{align*}
$$
Da $r^2 - y_{n - 1}^2$ er definitionen for $x_{n - 1}^2$, kan den indsættes:
$$
	x_n^2 = x_{n - 1}^2 - 2\cdot y_{n - 1} - 1
$$

Her fortsættes der, indtil vores $x_n$ er under vores halve radius. 
Disse punkter reflekteres gennem midten.

Metoden ovenfor giver kommatal som resultater, som evt. kan afrundes og bruges som pixelværdier.

## Integer-baseret
Definer en funktion $ER$, der beskriver et givent punkts afstand fra cirklen.
$$
	x^2 + y^2 = r^2 \\
	0 = x^2 + y^2 - r^2 
$$
Dette tages den absolutte værdi af, så et punkt der er indenfor ikke bliver valgt oftere end et udenfor.
$$
	F(x, y) = |x^2 + y^2 - r^2|
$$
For det første punkt, er vi perfekt på linjen, da $r^2 + 0^2 - r^2 = 0$:
$F(x_0, y_0) = 0$

Når et nyt punkt $(x_n, y_n)$ skal findes, undersøges følgende:
$$
	F(x_{n - 1} - 1, y_{n-1} + 1) < F(x_{n-1}, y_{n-1} + 1)
$$
..., altså om vi bevæger os negativ på x-aksen, eller vi kun bevæger os vertikalt. Hvis den holder er:
$$
	(x_n, y_n) = (x_{n - 1} - 1, y_{n - 1} + 1)
$$
Hvis den ikke holder:
$$
	(x_n, y_n) = (x_{n - 1}, y_{n - 1} + 1)
$$
### Nedkortning:
Uligheden $F(x_{n - 1} - 1, y_{n-1} + 1) < F(x_{n-1}, y_{n-1} + 1)$ kan forkortes, ved at se på de enkelte led:
Det første:
$$
\begin{align*}
	F(x, y) &= |x^2 + y^2 - r^2| \\
	F(x_{n - 1} - 1, y_{n-1} + 1) &= |(x_{n - 1} - 1 )^2 + (y_{n-1} + 1)^2 - r^2| \\
	&= |(x_{n - 1}^2 + 1 - 2\cdot x_{n - 1}) + (y_{n - 1}^2 + 1 - 2\cdot y_{n - 1}) - r^2|
\end{align*}
$$
Og for det andet:
$$
\begin{align*}
	F(x, y) &= |x^2 + y^2 - r^2| \\
	F(x_{n - 1}, y_{n-1} + 1) &= |(x_{n - 1})^2 + (y_{n-1} + 1)^2 - r^2| \\
	&= |x_{n - 1}^2 + (y_{n - 1}^2 + 1 - 2\cdot y_{n - 1}) - r^2|
\end{align*}
$$

Begge sider af uligheden opløftes i 2, siden begge er positive:
$$
\begin{align*}
	F(x_{n - 1} - 1, y_{n-1} + 1) &< F(x_{n-1}, y_{n-1} + 1) \\
	 ((x_{n - 1}^2 + 1 - 2\cdot x_{n - 1}) + (y_{n - 1}^2 + 1 - 2\cdot y_{n - 1}) - r^2)^2 &< (x_{n - 1}^2 + (y_{n - 1}^2 + 1 - 2\cdot y_{n - 1}) - r^2)^2 \\
\end{align*}
$$
(kill me)
Ryk rundt på led:
$$
\begin{align*}
	 &((x_{n - 1}^2 + 1 - 2\cdot x_{n - 1}) + (y_{n - 1}^2 + 1 - 2\cdot y_{n - 1}) - r^2)^2\\
	 &((x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1}) + (1 - 2x_{n - 1}))^2 \\
	 &(a + b)^2 = a^2+b^2+2ab \\
	 &(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})^2 + (1 - 2\cdot x_{n - 1})^2 \\
	 &~~~~~~+ 2\cdot(1 - 2\cdot x_{n - 1})(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})
\end{align*}
$$
Denne omrokering giver det fælles led $(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})^2$, som kan trækkes fra:
$$ - 2x
\begin{align*}
	&(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})^2 + (1 - 2\cdot x_{n - 1})^2
	 + 2\cdot(1 - 2\cdot x_{n - 1})(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1}) \\
	 <~&(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})^2
\end{align*}
$$
..., hvilket giver:
$$
\begin{align*}
	(1 - 2\cdot x_{n - 1})^2
	 + 2\cdot(1 - 2\cdot x_{n - 1})(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1}) < 0
\end{align*}
$$

Siden $x > 0$ og dermed $> 0.5$, da vi arbejder med heltal, er $(1 - 2\cdot x) < 0$. Når der divideres med $(1 - 2\cdot x)$ ændres ulighedens retning altså:
$$
\begin{align*}
	\frac {(1 - 2\cdot x_{n - 1})^2} {1 - 2\cdot x_{n - 1}}
	 + \frac {2\cdot(1 - 2\cdot x_{n - 1})(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1})} {1 - 2\cdot x_{n - 1}} &> 0 \\
	 2\cdot(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1}) + (1 + 2\cdot x_{n - 1}) &> 0
 \end{align*}
$$
I parentesen ses $(x_{n - 1}^2 + y_{n - 1}^2 - r^2)$, som jo er definitionen på $F$:
$$
\begin{align*}
	2\cdot(x_{n - 1}^2 + y_{n - 1}^2 + 1 - r^2 + 2\cdot y_{n - 1}) + (1 - 2\cdot x_{n - 1}) &> 0 \\
	2 \cdot (F(x_{n - 1}, y_{n - 1}) + 2\cdot y_{n - 1} + 1) + (1 - 2\cdot x_{n - 1}) &> 0
\end{align*}
$$


### Løbende udregning
I stedet for at regne hver eneste af disse uligheder ud, findes $\Delta$ af dette udtryk. Lad os kalde det D:
$$
\begin{align*}
	D_n &= 2\cdot (F(x_n, y_n) + 2 \cdot y_n + 1) + (1 - 2\cdot x_n) \\
	D_0 &= 2\cdot (r^2 + 0^2 - r^2 + 2 \cdot 0 + 1)+(1 - 2\cdot r) \\
	D_0 &= 2 + 1 - 2\cdot r \\
	D_0 &= 3 - 2\cdot r
\end{align*}
$$

Denne værdi kendes, og resten af tallene fra $D_1$ osv. udregnes baseret på denne. Dette gøres ved at isolere $F(x_n, y_n)$ i ligningen, så noget med $D_n$ kan sættes ind 
$$
\begin{align*}
	D_n &= 2\cdot (F(x_n, y_n) + 2 \cdot y_n + 1) + (1 - 2\cdot x_n) \\
	D_n - (1 - 2\cdot x_n) &= 2\cdot F(x_n, y_n) + 4 \cdot (y_n + 1) \\
	F(x_n, y_n) &= \frac {D_n - (1 - 2\cdot x_n) - 4\cdot (y_n + 1)} 2 \\
	F(x_n, y_n) &= \frac {D_n - 2\cdot x_n - 4\cdot y_n - 5} 2
\end{align*}
$$

Hvis $D > 0$: $y_n = y_{n - 1} + 1$ og $x_n = x_{n - 1} - 1$.
Hvis $D \leq 0$: $y_n = y_{n - 1} + 1$ og $x_n = x_{n - 1}$.

### X-ændrer sig:
For når:
$$
	x_n = x_{n - 1} - 1 \\
	y_n = y_{n - 1} + 1
$$
$$
	D_n = 2\cdot (F(x_{n - 1} - 1, y_{n - 1} + 1) + 2 \cdot (y_{n - 1} + 1) + 1) + (1 - 2\cdot (x_{n - 1} - 1))
$$
$$
\begin{align*}
	D_n &= 2\cdot (F(x_{n - 1} - 1, y_{n - 1} + 1) + 2 y_{n - 1} + 3) + 3 - 2\cdot x_{n - 1} \\
	D_n - D_{n - 1} &= 2\cdot (F(x_{n - 1} - 1, y_{n - 1} + 1) + 2 y_{n - 1} + 3) + 3 - 2\cdot x_{n - 1} \\ &~~~~- (2\cdot (F(x_{n - 1}, y_{n - 1}) + 2 \cdot y_{n - 1} + 1) + (1 - 2\cdot x_{n - 1})) \\
	&= 2 \cdot F(x_{n - 1} - 1, y_{n - 1} + 1) - 2\cdot (F(x_{n - 1}, y_{n - 1}) + 4\cdot y_{n - 1} - 4 \cdot y_{n - 1} \\ &~~~~ - 2\cdot x_{n - 1} + 2\cdot x_{n - 1} + 6 + 3 - 2 - 1
\end{align*}
$$
$$
\begin{align*}
	&= 2 \cdot F(x_{n - 1} - 1, y_{n - 1} + 1) - 2\cdot (F(x_{n - 1}, y_{n - 1}) + 3 \\
	&= 2 \cdot ((x - 1)^2 + (y + 1)^2 - r^2 - F(x_{n - 1}, y_{n - 1}) + 3) \\
	&= 2 \cdot (x^2 + 1 - 2x + y^2 + 1 + 2y - r^2 - F(x_{n - 1}, y_{n - 1}) + 3) \\
	&= 2 \cdot (x^2+y^2-r^2 - F(x_{n - 1}, y_{n - 1}) - 2x + 2y + 3 + 1 + 1) \\
	&= 2 \cdot (2y + 5 - 2x) \\
	D_n &= D_n = D_{n - 1} + 2 \cdot (2y + 5 - 2x) = D_{n - 1} + 4\cdot(y_{n - 1} - x_{n - 1}) + 10
\end{align*}
$$
Hvis $D > 0$ : $D_n = D_{n - 1} + 4\cdot(y_{n - 1} - x_{n - 1}) + 10$

### X-ændrer sig ikke:
For når:
$$
	x_n = x_{n - 1} \\
	y_n = y_{n - 1} + 1
$$
$$
\begin{align*}
	D_n &= 2\cdot (F(x_n, y_n) + 2\cdot y_n + 1) ~+ &(1 - 2\cdot x_n) \\
	D_n &= 2\cdot (F(x_{n - 1}, y_{n - 1} + 1) + 2 \cdot (y_{n - 1} + 1) + 1) ~+ &(1 - 2\cdot x_{n - 1})
\end{align*}
$$
$$
\begin{align*}
	D_n &= 2\cdot (F(x_{n - 1}, y_{n - 1} + 1) + 2 y_{n - 1} + 3) + 1 - 2\cdot x_{n - 1} \\
	D_n - D_{n - 1} &= 2\cdot (F(x_{n - 1}, y_{n - 1} + 1) + 2 y_{n - 1} + 3) + 1 - 2\cdot x_{n - 1} \\ &~~~~- (2\cdot (F(x_{n - 1}, y_{n - 1}) + 2 \cdot y_{n - 1} + 1) + (1 - 2\cdot x_{n - 1})) \\
	&= 2 \cdot F(x_{n - 1}, y_{n - 1} + 1) - 2\cdot (F(x_{n - 1}, y_{n - 1}) + 4\cdot y_{n - 1} - 4 \cdot y_{n - 1} \\ &~~~~ - 2\cdot x_{n - 1} + 2\cdot x_{n - 1} + 6 + 1 - 2 - 1
\end{align*}
$$
$$
\begin{align*}
	&= 2 \cdot F(x_{n - 1}, y_{n - 1} + 1) - 2\cdot (F(x_{n - 1}, y_{n - 1}) + 4 \\
	&= 2 \cdot (x^2 + (y + 1)^2 - r^2 - F(x_{n - 1}, y_{n - 1}) + 2) \\
	&= 2 \cdot (x^2 + y^2 + 1 + 2y - r^2 - F(x_{n - 1}, y_{n - 1}) + 2) \\
	&= 2 \cdot (x^2+y^2-r^2 - F(x_{n - 1}, y_{n - 1}) + 2y + 2 + 1) \\
	&= 2 \cdot (2y + 3) \\
	D_n &= D_{n - 1} + 2 \cdot (2y + 3)
\end{align*}
$$

Hvis $D \leq 0$ : $D_n = D_{n - 1} + 4 \cdot y_{n - 1} + 6$

## Algoritmen, step by step
Start ved $x_0 = r, y_0 = 0$.
Udregn $D_0 = 3 - 2\cdot r$
Start loop, imens $x > y$ (tegn oktant 45 grader fra x-aksen)
- Tegn 8 pixels
- Hvis $D_{n - 1} > 0$: $D_n = D_{n - 1} + 4\cdot(y_{n - 1} - x_{n - 1}) + 10$
- Hvis $D_{n - 1} \leq 0$: $D_n = D_{n - 1} + 4 \cdot y_{n - 1} + 6$
