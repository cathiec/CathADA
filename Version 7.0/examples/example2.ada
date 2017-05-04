STATES
q0 q1 q2 q3 q4 q5 q6

INITIAL
q0

FINAL
q5

SYMBOLS
a b c

VARIABLES
x y

TRANSITIONS
a q0
(or (and (and q1 (<= x1 8)) (and q2 (> x1 3))) (and q1 (> x1 100) q3))
b q1
(and q3 (>= x1 6) (= x1 (+ x0 1)))
b q2
(and q4 (<= x1 7) (= x1 (+ x0 1)))
b q3
q6
c q3
(and q5 (> x1 7) (= x1 (+ x0 1)))
c q4
(and q5 (< x1 10) (= x1 (+ x0 1)))
c q6
q5
