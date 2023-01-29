/*
#include "pid.h"
#include <stdio.h>

int main() {

    PID pid = PID(0.1, 100, -100, 0.1, 0.01, 0.5);

    double val = 20;
    for (int i = 0; i < 100; i++) {
        double inc = pid.calculate(0, val);
        printf("val:% 7.3f inc:% 7.3f\n", val, inc);
        val += inc;
    }

    return 0;
}   */

#include <ros/ros.h>
#include <motor_test/motor_node.h>
#include <my_msgs/CameraData.h>
#include <my_msgs/SensorData.h>
#include <my_msgs/SignalData.h>
#include <iostream>

////////////////////////////////////
#define X_BOUND_1 -100
#define X_BOUND_2 100
#define BIG_SIZE 500
#define SENSOR_LIMIT 50
///////////////////////////////////// define value, 일단 임의값으로 해놓음

/////////////////////////////
bool person_detect = false;
bool sign_detect = false;

float person_x = 0;
float person_size = 0;
float sign_x = 0;
float sign_size = 0;
/////////////////////////////// cam data


/////////////////////////// 
float FrontSensor = 0;
float LeftSensor = 0;
float RightSensor = 0;
float BackSensor = 0;
////////////////////////// sensor data


///////////////
int state = 0; // display data
///////////////


int sequence = 0; // sequence 0 to 3


void CamDataCallback(const my_msgs::CameraData& msg) { // function which subscribes from camera
    person_detect = msg.p_detect;
    person_x = msg.p_x;
    person_size = msg.p_size;
    sign_detect = msg.s_detect;
    sign_x = msg.s_x; 
    sign_size = msg.s_size; //동현이형이랑 메시지명 맞춰야되는 부분 p_detect, p_x 등 
}

void SensorDataCallback(const my_msgs::SensorData& msg) {
    FrontSensor = msg.front;
    LeftSensor = msg.left;
    RightSensor = msg.right;
    BackSensor = msg.back;
}

void SignalDataCallback(const my_msgs::SignalData& msg) {
    state = msg.data;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motor_node");
  ros::NodeHandle nh;
  Initialize();
  ros::Publisher signal_pub = nh.advertise<my_msgs::SignalData>("/signal/topic", 10);
  ros::Subscriber signal_sub = nh.subscribe("/signal/topic2", 10, SignalDataCallback);
  ros::Subscriber sensor_sub = nh.subscribe("/sensor/topic", 10, SensorDataCallback);
  ros::Subscriber camera_sub = nh.subscribe("/camera/topic", 10, CamDataCallback); //camera topic sub
  ros::Rate loop_rate(Control_cycle);
  my_msgs::SignalData signal_msg;

  while(ros::ok())
  {
      
    switch(sequence)
    {
        case 0:

            camera_sub;
            sensor_sub; //subscribe camera and sensor data

            if(person_detect) {        // object-detection 여부에 대한 bool형 - 사람
                Motor_Controller(1, true, 0);
                Motor_Controller(2, true, 0);
                sequnece = sequence + 1;    // 다음 단계
                break;
            }

            Motor_Controller(1, true, 30);
            Motor_Controller(2, false, 30);

        case 1:

            camera_sub;
            sensor_sub;

            if(FrontSensor < SENSOR_LIMIT || person_size > BIG_SIZE){      // 전진 센서 작동 or 사이즈 커졌을 때
                Motor_Controller(1, true, 0);
                Motor_Controller(2, true, 0);
                signal_msg.data = 1 // 사람에게 도달했을 때 디스플레이에 데이터 1로 신호 pub
                signal_pub.publish(signal_msg);      // pub~~~ -> '스크린 Rpi'로 데이터 넘겨줌(interaction start)
                
                person_detect = 0;
                person_x = 0;
                person_size = 0; //camera data 초기화

                sequence = sequence + 1;    // 다음 단계
                break;
            }

            else
            {
                if(person_x > X_BOUND_1 && person_x < X_BOUND_2){     // x_person: detection된 사람 화면에서 x값, v1,v2는 바운더리, 내에 있을 때 전진
                Motor_Controller(1, true, 50);
                Motor_Controller(2, true, 50);
                }
                else if(x <= X_BOUND_1){    // 왼쪽으로 회전
                    Motor_Controller(1, true, 30);
                    Motor_Controller(2, true, 50);      // pid제어를 통해 값 변화에 따라 회전값 증가 필요
                }
                else(x > X_BOUND_2){
                    Motor_Controller(1, true, 50);
                    Motor_Controller(2, true, 30);
                }
            }

        case 2:     // Screen interaction  나중에 없애도 됌.
            
            signal_sub;

            if (state == 9999) // 9999 = display's end_signal
            {
                sequence++;; // 다음단계
                break;
            }

            /*
            if (x < V1) {    // 제자리 왼쪽으로 회전
                Motor_Controller(1, true, 30);
                Motor_Controller(2, false, 30);      // pid제어를 통해 값 변화에 따라 회전값 증가 필요
            }
            
            else(x > V2){   // 제자리 오른쪽 회전
                Motor_Controller(1, false, 30);
                Motor_Controller(2, true, 30);      // 반복문이라 생각 조금 더
             */
        case 3:

            camera_sub;
            sensor_sub;

            if(sign_detect)     // 표지판 detection
            {
                if(x_sign == V)   // 표지판 정면(V)에 두기 //지환 : V가 무슨 값 말하는건지 모르겠어어 일단 놔둠, 필요하면 위에 define으로 임의값 정의
                {
                    if(size_sign > V)
                    {
                        Motor_Controller(1, true, 30);
                        Motor_Controller(2, true, 30);
                    }
                    else       // 크기가 작다, 거리가 멀다 => 향해서 전진
                    {
                        Motor_Controller(1, true, 0);
                        Motor_Controller(2, true, 0);
                        
                        person_detect = 0;
                        person_x = 0;
                        person_size = 0; //camera data 초기화

                        sequence = 0; //sequence 초기화


                        break;
                    }
                }
                else
                {
                    Motor_Controller(1, true, 20);
                    Motor_Controller(2, false, 20);     // 움직이던 방향대로 조금 회전
                }
            }
            else
            {
                Motor_Controller(1, true, 30);
                Motor_Controller(2, false, 30);     // detection 하기 전까지 한쪽 방향으로 회전
            }

        default:

            person_detect = 0;
            person_x = 0;
            person_size = 0; //camera data 초기화
            
            sequence = 0;
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  Motor_Controller(1, true, 0);
  Motor_Controller(2, true, 0); //노드 종료시 무조건 모터정지
  return 0;
}






