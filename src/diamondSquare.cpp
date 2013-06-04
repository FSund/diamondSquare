#include <src/diamondSquare.h>

DiamondSquare::DiamondSquare(const int power2, long idum):
    n(power2),
    L0(pow(2,n)),
    L(L0),
    idum(idum),
    R(zeros<mat>(L0+1,L0+1))
{
}

mat DiamondSquare::generate(const double H, const double corners)
{
//    R(0,0) = 2.0;
//    R(0,L0) = 2.0;
//    R(L0,0) = 2.0;
//    R(L0,L0) = 2.0;

//    R(0,0) = ran2(&idum);
//    R(0,L0) = ran2(&idum);
//    R(L0,0) = ran2(&idum);
//    R(L0,L0) = ran2(&idum);

    if (corners == 42) {
        R(0,0) = ran2(&idum);
        R(0,L0) = ran2(&idum);
        R(L0,0) = ran2(&idum);
        R(L0,L0) = ran2(&idum);
    }
    else {
        R(0,0) = corners;
        R(0,L0) = R(0,0);
        R(L0,0) = R(0,0);
        R(L0,L0) = R(0,0);
    }

    L = L0;
    double randrange = 1.0;
    double randfac = pow(2.0, -H);
    int len;
    umat rpos;
    int l;
    for (int i = 0; i < n; i++) {
        L = L0*pow(2, -i);
        len = L0/L;

        rpos.set_size(2, len*len);
        l = 0;
        for (int j = 0; j < len; j++) {
            for (int k = 0; k < len; k++) {
                rpos(0,l) = j*L;
                rpos(1,l) = k*L;
                l += 1;
            }
        }

        /* squares */
        for (int j = 0; j < len*len; j++)
            square(R, rpos.col(j), L0, L, randrange);

        /* diamonds */
        for (int j = 0; j < len*len; j++)
            diamond(R, rpos.col(j), L0, L, randrange);

        rpos.set_size(2, len);
        /* bottom diamonds */
        for (int j = 0; j < len; j++) {
            rpos(0,j) = L0 - L;
            rpos(1,j) = j*L;
        }
        for (int j = 0; j < len; j++)
            bottom(R, rpos.col(j), L0, L, randrange);

        /* right diamonds */
        for (int j = 0; j < len; j++) {
            rpos(0,j) = j*L;
            rpos(1,j) = L0 - L;
        }
        for (int j = 0; j < len; j++)
            right(R, rpos.col(j), L0, L, randrange);

        randrange *= randfac;
    }

    return R;
}

void DiamondSquare::square(mat &R, const uvec &pos, const int &L0, const int &L, const double &randrange)
{
    uvec xpos(4,1), ypos(4,1);
//    vec xadd(4,1), yadd(4,1);
    for (int i = 0; i < 4; i++) {
        xpos(i) = pos(0);
        ypos(i) = pos(1);
    }

//    xpos(0) += 0.0;
//    xpos(1) += 0.0;
    xpos(2) += L;
    xpos(3) += L;
//    ypos(0) += 0.0;
    ypos(1) += L;
//    ypos(2) += 0.0;
    ypos(3) += L;

    double center = 0.0;
    for (int i = 0; i < 4; i++) {
        center += R(xpos(i), ypos(i));
    }
    R(pos(0) + L/2, pos(1) + L/2) += center*0.25 + (ran2(&idum) - 0.5)*randrange;
//    R(pos(0) + L/2, pos(1) + L/2) += center*0.25;
}

void DiamondSquare::diamond(mat &R, const uvec pos, const int L0, const int L, const double randrange)
{
    ivec xpos(4,1), ypos(4,1);
    double center;
    int len;

    /* left */
    len = 4;
    for (int i = 0; i < 4; i++) {
        xpos(i) = pos(0);
        ypos(i) = pos(1);
    }

    xpos(0) += L/2; // OK WITH BOUNDARIES
    xpos(1) += L/2;
    xpos(2) += 0;
    xpos(3) += L;

//    ypos(0) = (pos(1) == 0) ? 0 : pos(1) - L/2; // BOUNDARIES
    if (pos(1) == 0) { // BOUNDARIES
        len = 3;
        ypos(0) = 0;
    } else {
        ypos(0) = pos(1) - L/2;
    }
    ypos(1) += L/2;
    ypos(2) += 0;
    ypos(3) += 0;

    center = 0.0;
    for (int i = 0; i < 4; i++) {
        center += R(xpos(i), ypos(i));
    }
    R(pos(0) + L/2, pos(1)) += center/len + (ran2(&idum) - 0.5)*randrange;
//    R(pos(0) + L/2, pos(1)) += center/len;


    /* top */
    len = 4;
    for (int i = 0; i < 4; i++) {
        xpos(i) = pos(0);
        ypos(i) = pos(1);
    }

    if (pos(0) == 0) {
        xpos(0) = 0;
        len = 3;
    } else {
        xpos(0) = pos(0) - L/2;
    }
//    xpos(0) = (pos(0) == 0) ? pos(0) : pos(0) - L/2; // boundaries
    xpos(1) += L/2;
    xpos(2) += 0;
    xpos(3) += 0;

    ypos(0) += L/2; // OK with boundaries
    ypos(1) += L/2;
    ypos(2) += 0;
    ypos(3) += L;

    center = 0.0;
    for (int i = 0; i < 4; i++) {
        center += R(xpos(i), ypos(i));
    }
    R(pos(0), pos(1) + L/2) += center/len + (ran2(&idum) - 0.5)*randrange;
//    R(pos(0), pos(1) + L/2) += center/len;
}

void DiamondSquare::bottom(mat &R, const uvec pos, const int L0, const int L, const double randrange)
{
    uvec xpos(3,1), ypos(3,1);
    double center;

    /* bottom */
    for (int i = 0; i < 3; i++) {
        xpos(i) = pos(0);
        ypos(i) = pos(1);
    }

    xpos(0) += L;
    xpos(1) += L/2;
    xpos(2) += L;

    ypos(0) += 0;
    ypos(1) += L/2;
    ypos(2) += L;

    center = 0.0;
    for (int i = 0; i < 3; i++) {
        center += R(xpos(i), ypos(i));
    }
    R(pos(0) + L, pos(1) + L/2) += center/3.0 + (ran2(&idum) - 0.5)*randrange;
//    R(pos(0) + L, pos(1) + L/2) += center/3.0;
}

void DiamondSquare::right(mat &R, const uvec pos, const int L0, const int L, const double randrange)
{
    uvec xpos(3,1), ypos(3,1);
    double center;

    /* right */
    for (int i = 0; i < 3; i++) {
        xpos(i) = pos(0);
        ypos(i) = pos(1);
    }

    xpos(0) += 0; // OK WITH BOUNDARIES
    xpos(1) += L/2;
    xpos(2) += L;

    ypos(0) += L;
    ypos(1) += L/2;
    ypos(2) += L;

    center = 0.0;
    for (int i = 0; i < 3; i++) {
        center += R(xpos(i), ypos(i));
    }
    R(pos(0) + L/2, pos(1) + L) += center/3.0 + (ran2(&idum) - 0.5)*randrange;
//    R(pos(0) + L/2, pos(1) + L) += center/3.0;
}

