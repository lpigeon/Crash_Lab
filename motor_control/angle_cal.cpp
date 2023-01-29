
#define a   // length
#define b
#define c   // sqrt(pow(a, 2) + pow(b,2)), 빗변

#define alpha
#define beta    // degree or radian

float d_1    // distance with sign1
float d_2    // distance with sign2, need to calculate

float theta_1   // degree between sign1 and sign2, 측정값


float theta_n   // 
float theta_q

float delta_1
float delta_2

float delta_x
float delta_y
float delta_d


delta_1 = asin(d2 * sin(theta_1) / c);      // need to convert radian to degree
delat_2 = 90 - delta_1 - theta_1;

delta_x = d_1 * sin(delta_1);
delta_y = d_1 * cos(delat_1) - b;
delta_d = sqrt(pow(delta_x, 2) + pow(delta_y,2));

theta_n = atan(delta_x / delta_y);

theta_q = theta_n - delta_2;

// theta_Turn(theta_q);
// distance_go(delta_d)







