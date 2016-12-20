STATES
q0 q1

INITIAL
q0

SYMBOLS
a

VARIABLES
x

TRANSITIONS
a q0
(and (= x1 4) q1)
a q1
(or (< x0 4) (and (= x1 (+ x0 1)) q1))
