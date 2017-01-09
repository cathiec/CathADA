STATES
q0 q1 q2 q3 q4

INITIAL
q0

FINAL
q4

SYMBOLS
a

VARIABLES
x

TRANSITIONS
a q0
(and (= x1 3) q1)
a q1
(and q2 q3 (= x1 (+ x0 1)))
a q2
(or (and q1 (= x1 x0)) (and q4 (< x0 3)))
a q3
(or (and q1 (= x1 x0)) (and q4 (< x0 3)))
