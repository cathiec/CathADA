STATES
q0 q1 q2 q3

INITIAL
q0

FINAL
q3

SYMBOLS
tau

VARIABLES
a b c i j l x

TRANSITIONS
tau q0
(and q1 (= a0 x0) (= c0 a1) (<= 0 i0) (<= (+ 2 i0) l0) (= j0 0) (= j1 (+ j0 1)) (= i1 i0) (= l0 l1) (= x0 x1))
tau q1
(or (and q1 (= c0 a1) (<= j0 (- i0 1)) (= j1 (+ j0 1)) (= i1 i0) (= l0 l1) (= x0 x1)) (and q2 (= c0 a1) (= j0 (+ i0 0)) (= j1 (+ j0 1)) (= i1 i0) (= l0 l1) (= x0 x1)))
tau q2
(or (and q2 (= c0 a0) (<= j0 (- l0 1)) (= j1 (+ j0 1)) (= i1 i0) (= l0 l1) (= x0 x1)) (and q3 (= j0 l0)))
