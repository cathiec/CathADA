STATES
q0 q1 q2 q3 q4 q5

INITIAL
q0

FINAL
q2

SYMBOLS
a b

VARIABLES
L new old

TRANSITIONS
a q0
(and q1 (= L1 0) (= new1 new0) (= old1 old0))
a q1
(and q2 (not (= L0 0)) (= L1 L0) (= new1 new0) (= old1 old0))
b q1
(and q3 (= L1 1) (= new1 new0) (= old1 new0))
a q3
(and q4 (= L1 L0) (= new1 new0) (= old1 old0))
b q3
(and q4 (= L1 0) (= new1 new0) (= old1 old0))
a q4
(and q1 (not (= new0 old0)) (= L1 L0) (= new1 new0) (= old1 old0))
b q4
(and q5 (= new0 old0) (= L1 L0) (= new1 new0) (= old1 old0))
