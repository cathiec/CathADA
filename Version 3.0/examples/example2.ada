STATES
q0 q1 q2 q3 q4 q5

INITIAL
q0

FINAL
q5

SYMBOLS
a b c d

VARIABLES
x

TRANSITIONS
d q1
(and q3 q4)
d q2
(> x0 0)
d q3
q5
d q4
q5
a q0
(and (and q1 (<= x0 8) (= x1 (+ x0 1))) (and q2 (> x0 3) (= x1 (+ x0 1))))
b q1
(and q3 (>= x0 6) (= x1 (+ x0 1)))
b q2
(and q4 (<= x0 7) (= x1 (+ x0 1)))
c q3
(and q5 (> x0 8))
c q4
(and q5 (< x0 10))
