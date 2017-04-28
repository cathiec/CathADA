STATES
q0 q1 q2 q3 q4 q5 q6 q7 q8

INITIAL
q0

FINAL
q7 q8

SYMBOLS
a b c d

VARIABLES
x

TRANSITIONS
a q0
(or (and q1 (> x1 1)) (and q2 (> x1 2)))
b q1
(and q3 (> x1 3))
b q2
(and q4 (> x1 4))
c q3
(or (and q5 (> x1 5)) (and q6 (> x1 6)))
c q4
(and q6 (> x1 7))
d q5
(and q7 (> x1 8))
d q6
(or (and q7 (> x1 9)) (and q8 (> x1 10)))
