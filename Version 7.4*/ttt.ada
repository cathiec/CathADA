STATES
q0 q1 q2 q3 q4

INITIAL
q0

FINAL
q3 q4

SYMBOLS
a b

VARIABLES
x y

TRANSITIONS
a q0
(and q1 q2 (= x1 0) (= y1 0))
a q1
(and q1 q2 (= x1 (+ y0 1)) (= y1 (+ x0 1)))
b q1
(and q3 (>= x0 y0))
a q2
(and q2 (> x1 x0) (> y1 y0))
b q2
(and q4 (> x0 y0))
