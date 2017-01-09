STATES
q0 q1 q2

INITIAL
q0

FINAL
q2

SYMBOLS
a b

VARIABLES
x

TRANSITIONS
a q0
(and (= x1 3) q1)
a q1
(and (= x1 (+ x0 1)) q1)
b q1
(and q2 (< x0 4))
