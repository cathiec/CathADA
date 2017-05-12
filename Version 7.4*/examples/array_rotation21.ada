STATES
q0 q1 q2 q3 q4 q5 q6 q7

INITIAL
q0

FINAL
q1

SYMBOLS
x

VARIABLES
a0 lp a1 a2 xp l N I x ip i

TRANSITIONS
x q2
(or (and q3 (= N1 N0) (= xp1 xp0) (= l1 l0) (= x1 x0) (= lp1 lp0) (= I1 (+ i1 1)) (= a11 a20) (= i1 i0) (= i1 I0) (= ip1 ip0) (>= N1 (+ i1 2))) (and q1 (= x1 x0) (= l1 l0) (= I1 N1) (= N1 N0) (= xp1 xp0) (= lp1 lp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q2 (= lp1 lp0) (= N1 N0) (= x1 x0) (= xp1 xp0) (= l1 l0) (= a11 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1)) (<= I1 i1)))
x q4
(or (and q4 (= a10 a20) (= a00 a20) (= x1 x0) (= xp1 xp0) (= l1 l0) (= N1 N0) (= lp1 lp0) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- l1 1)) (<= I1 (- N1 1))) (and q1 (= l1 l0) (= I1 N1) (= N1 N0) (= xp1 xp0) (= x1 x0) (= a10 a20) (= a00 a20) (= lp1 lp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q5 (= a10 a20) (= a00 a20) (= x1 x0) (= xp1 xp0) (= lp1 lp0) (= I1 l1) (= l1 l0) (= N1 N0) (= l1 (+ I0 1)) (= i1 i0) (= ip1 ip0) (<= l1 (- N1 1))))
x q3
(or (and q3 (= xp1 xp0) (= l1 l0) (= N1 N0) (= lp1 lp0) (= x1 x0) (= a10 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- l1 1)) (<= I1 (- N1 1))) (and q1 (= lp1 lp0) (= x1 x0) (= xp1 xp0) (= a10 a20) (= l1 l0) (= I1 N1) (= N1 N0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q5 (= x1 x0) (= lp1 lp0) (= a10 a20) (= I1 l1) (= l1 l0) (= N1 N0) (= xp1 xp0) (= l1 (+ I0 1)) (= i1 i0) (= ip1 ip0) (<= l1 (- N1 1))))
x q5
(or (and q1 (= x1 x0) (= xp1 xp0) (= I1 N1) (= N1 N0) (= l1 l0) (= lp1 lp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q5 (= lp1 lp0) (= xp1 xp0) (= l1 l0) (= x1 x0) (= N1 N0) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1))))
x q0
(or (and q6 (= I1 1) (= I0 0) (= xp1 x1) (= xp1 xp0) (= xp1 x0) (= xp1 a00) (= N1 N0) (= lp1 l0) (= lp1 lp0) (= ip1 (+ i1 1)) (= ip1 (+ i0 1)) (= l1 lp1) (= ip1 ip0) (= a01 a10) (= a01 a20) (<= ip1 (- lp1 1)) (>= ip1 3) (>= N1 lp1)) (and q4 (= ip1 1) (= I1 1) (= i0 0) (= i1 0) (= I0 0) (= xp1 x1) (= xp1 xp0) (= xp1 x0) (= xp1 a10) (= xp1 a00) (= N1 N0) (= lp1 l0) (= lp1 lp0) (= ip0 1) (= a11 a20) (= l1 lp1) (>= lp1 2) (>= N1 lp1)) (and q7 (= I1 1) (= i1 1) (= I0 0) (= i0 1) (= xp1 a00) (= xp1 x0) (= xp1 xp0) (= xp1 x1) (= N1 N0) (= lp1 l0) (= lp1 lp0) (= ip0 2) (= ip1 2) (= l1 lp1) (= a01 a10) (= a01 a20) (>= lp1 3) (>= N1 lp1)))
x q7
(or (and q4 (= lp1 lp0) (= x1 x0) (= l1 l0) (= N1 N0) (= xp1 xp0) (= I1 (+ i1 1)) (= a11 a20) (= i1 i0) (= i1 I0) (= ip1 ip0) (= a00 a10) (>= l1 (+ i1 2)) (>= N1 (+ i1 2))) (and q3 (= lp1 lp0) (= x1 x0) (= N1 N0) (= l1 l0) (= I1 l1) (= xp1 xp0) (= l1 (+ i1 1)) (= a11 a20) (= l1 (+ I0 1)) (= l1 (+ i0 1)) (= ip1 ip0) (= a00 a10) (<= l1 (- N1 1))) (and q2 (= x1 x0) (= lp1 lp0) (= N1 N0) (= l1 l0) (= I1 l1) (= xp1 xp0) (= a11 a20) (= l1 (+ I0 1)) (= i1 i0) (= ip1 ip0) (= a00 a10) (<= l1 (- N1 1)) (<= l1 i1)) (and q1 (= xp1 xp0) (= I1 N1) (= N1 N0) (= x1 x0) (= lp1 lp0) (= l1 l0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0) (= a00 a10)) (and q7 (= lp1 lp0) (= xp1 xp0) (= N1 N0) (= l1 l0) (= x1 x0) (= a11 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (= a00 a10) (<= I1 (- N1 1)) (<= I1 (- l1 1)) (<= I1 i1)))
x q6
(or (and q7 (= a01 a10) (= lp1 lp0) (= x1 x0) (= l1 l0) (= N1 N0) (= xp1 xp0) (= a01 a20) (= I1 i1) (= i1 i0) (= i1 (+ I0 1)) (= ip1 ip0) (>= N1 (+ i1 1))) (and q6 (= a01 a10) (= l1 l0) (= lp1 lp0) (= x1 x0) (= xp1 xp0) (= N1 N0) (= a01 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1)) (<= I1 (- i1 1))) (and q1 (= x1 x0) (= lp1 lp0) (= I1 N1) (= N1 N0) (= l1 l0) (= xp1 xp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)))
