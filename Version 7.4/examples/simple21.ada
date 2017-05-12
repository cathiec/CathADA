STATES
q0 q1 q2

INITIAL
q0

FINAL
q0

SYMBOLS
a b

VARIABLES
x y

TRANSITIONS
a q0
(and q1 (= x1 (+ x0 1)) (= y1 y0))
b q1
(and q0 (= x0 x1) (= y1 (+ y0 1)))
