STATES
q0 q1 q2

INITIAL
q0

SYMBOLS
a epsilon

VARIABLES
i n j a b

TRANSITIONS
a q0
(and q1 (< 0 n1) (= 0 j1 ) (< i1 n1) (<= 0 i1))
a q1
(or (and q1 (< j0 i0 ) (= b1 a0) (= j1 (+ j0 1)) (= n0 n1) (= i0 i1)) (and q2 (= j0 i0 ) (= j1 (+ j0 1)) (= n0 n1) (= i0 i1)))
a q2
(and q2 (< j0 n0) (= j1 (+ j0 1)) (= n0 n1) (= i0 i1))
epsilon q0
q0
epsilon q1
q1
epsilon q2
true
