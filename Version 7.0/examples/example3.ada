STATES
q0 q1 q2 q3 q4 q5

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
(and (= x1 4) q1)
a q1
(or (and q2 (< x0 4)) (and (= x1 x0) q3))
a q3
(or (and q4 (< x0 4)) (and (= x1 x0) q5))
