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
(or (and q1 (< x0 x1) (<= y1 y0)) (and q2 (>= (- x1 y1) (- x0 y0))))
b q1
(and q0 (>= x0 x1) (> y1 y0))
b q2
(and q0 (<= (- x1 y1) (- x0 y0)))
