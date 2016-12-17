STATES
q0 q1 q2 q3 q4 q5 q6

SYMBOLS
a b

VARIABLES
x

TRANSITIONS
a q0
(and (= x1 4) q1)
a q1
(and (= x1 (+ x0 1)) q1)
b q1
(< x0 4)
