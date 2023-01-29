#define d_0 1050 // half the distance between sign1 and 3, 10.5m(1050cm)

///////////////////////// set return valiable
float d_1 = 0;      // distance with sign1
float d_2 = 0;      // distance with sign2, need to calculate

float theta_12 = 0; // angle sign1 to 2
float theta_21 = 0; // angle sign2 to 1
float theta_alpha = 0;
float theta_beta = 0;

float theta_delta = 0;
float theta_turn = 0;

float delta_x = 0;
float delta_y = 0;
float delta_d = 0;
//////////////////////////////////////////////////
//////////////////////////////////////////////////
int flag = 0;

bool sing1_detect = false;
float sing1_x = 0;

bool sing2_detect = false;
float sign2_x = 0;

int First_Encoder = 0;
int Second_Encoder = 0;
int Change_Encoder = 0;
//////////////////////////////////////////////////
void Init_Cal_Theta()
{
    int flag = 0;

    bool sing1_detect = false;
    float sing1_x = 0;

    bool sing2_detect = false;
    float sing2_x = 0;

    int First_Encoder = 0;
    int Second_Encoder = 0;
    int Change_Encoder = 0;
}

float Cal_Theta(int num1, int num2)    // function of calculate theta
{
    switch (num1)
    {
    case 1:
        sing1_detect = sign1_detect;
        sing1_x = sign1_x;
        break;
    case 2:
        sing1_detect = sign2_detect;
        sing1_x = sign2_x;
        break;
    case 3:
        sing1_detect = sign3_detect;
        sing1_x = sign3_x;
        break;
    case 4:
        sing1_detect = sign4_detect;
        sing1_x = sign4_x;
        break;
    }

    switch (num2)
    {
    case 1:
        sing2_detect = sign1_detect;
        sing2_x = sign1_x;
        break;
    case 2:
        sing2_detect = sign2_detect;
        sing2_x = sign2_x;
        break;
    case 3:
        sing2_detect = sign3_detect;
        sing2_x = sign3_x;
        break;
    case 4:
        sing2_detect = sign4_detect;
        sing2_x = sign4_x;
        break;
    }

    if (flag == 0)
    {
        Motor_Controller(1, true, 30);
        Motor_Controller(2, true, 30);

        if (sing1_detect == true && sing1_x == BOUND_MID)
        {
            First_Encoder = Motor1_Encoder_Sum;
            flag++;
        }
    }

    if (flag == 1)
    {
        Motor_Controller(1, false, 30);
        Motor_Controller(2, false, 30);

        if (sing2_detect == true && sing2_x == BOUND_MID)
        {
            Second_Encoder = Motor1_Encoder_Sum;
            flag++;
        }
    }

    if (flag == 2)
    {
        Change_Encoder = First_Encoder - Second_Encoder;
        theta = Change_Encoder / (Encoder_resolution * 4 / 360) / (Robot_round / Wheel_round);
        return abs(theta);
    }
}

void init_return(){
    d_1 = 0;
    d_2 = 0;

    theta_12 = 0;
    theta_21 = 0;
    theta_alpha = 0;
    theta_beta = 0;

    theta_delta = 0;
    theta_turn = 0;
    turn_dir = 30;      // turn direction and vel, default is cw(+)

    delta_x = 0;
    delta_y = 0;
    delta_d = 0;
}

void reutrn_method()
{
    d_1 = ?;    // need to calculate by size of sign1 & 2
    d_2 = ?;   // arbitrary result yet, need to fix

    if(sign1_x == BOUND_MID)
    {
        theta_12 = Cal_Theta(1, 2);

        if(theta_12 > 180){     // when it is looking for sign2 on left, turn ccw
            theta_12 = 360 - theta_12;
            turn_dir = turn_dir * (-1);
            // set theta_12 is between 0'~180'(degree)
        }

        // when it is looking for sign2 on right, turn cw
        theta_beta = asin(d_1 * sin(theta_12) / 2*d_0);

        delta_x = d_2 * sin(theta_beta);
        delta_y = d_2 * cos(theta_beta) - d_0;
        delta_d = sqrt(pow(delta_x, 2) + pow(delta_y,2));

        theta_delta = atan(delta_y / delta_x);
        theta_turn = 90 - theta_beta - theta_delta;     // degree

        Theta_Turn(theta_turn, turn_dir)      //(double theta, int PWM), turn cw
        Distance_Go(delta_d, 50);       //(double distance, int PWM)

    }
    else if(sign2_x == BOUND_MID)
    {
        theta_21 = Cal_Theta(2, 1);

        if(theta_21 > 180){     // when it is lookin for sign1 on right, turn ccw
            theta_21 = 360 - theta_21;
            turn_dir = turn_dir * (-1);
            // set theta_21 is between 0'~180'(degree)
        }

        // when it is looking for sign1 on left, turn cw
        theta_alpha = asin(d_1 * sin(theta_21) / 2*d_0);

        delta_x = d_1 * sin(theta_alpha);
        delta_y = d_1 * cos(theta_alpha) - d_0;
        delta_d = sqrt(pow(delta_x, 2) + pow(delta_y,2));

        theta_delta = atan(delta_y / delta_x);
        theta_turn = 90 - theta_alpha - theta_delta;

        Theta_Turn(theta_turn, turn_dir);     //(double theta, int PWM), ccw
        Distance_Go(delta_d, 50);       //(double distance, int PWM)

    }
}


if(sequence ==3)
{
    // before detecting sign1 or 2, turn ccw
    Motor_Controller(1, true, 30);
    Motor_Controller(2, true, 30);

    if(sign1_x == BOUND_MID || sign2_x == BOUND_MID)
    {
        ////////////
        return_method();
        // delay?
        ////////////

        init_return();
        Init_Cal_Theta();
        sequence = 0;   // go to init sequence
    }
}

