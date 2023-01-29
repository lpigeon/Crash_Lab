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



int main(int argc, char** argv)
{
  ros::init(argc, argv, "motor_main");
  ros::NodeHandle nh;
  ros::Publisher pub_display = nh.advertise<my_msgs::SignalData>("/signal/topic", 10);
  ros::Subscriber sub_display = nh.subscribe("/signal/topic2", 10, NumberCallback);
  Initialize();
  ros::Rate loop_rate(10);

  while(ros::ok())
  {
    switch(sequence)
    {
        case 0:
            if(detect1) {        // object-detection 여부에 대한 bool형 - 사람
                sequnece = sequence + 1;    // 다음 단계
                Motor_Controller(1, true, 0);
                Motor_Controller(2, true, 0);
                break;
            }
            Motor_Controller(1, true, 30);
            Motor_Controller(2, false, 30);
        case 1:
            if(sensor_data.ultra_sonic_3 < V || size_person > V){      // 전진 센서 작동 or 사이즈 커졌을 때
                Motor_Controller(1, true, 0);
                Motor_Controller(2, true, 0);
                sequence = sequence + 1;    // 다음 단계
                pub_display.publish(sequence);      // pub~~~ -> '스크린 Rpi'로 데이터 넘겨줌(interaction start)
                break;
            }
            else
            {
                if(x_person > V1 && x_person < V2){     // x_person: detection된 사람 화면에서 x값, v1,v2는 바운더리, 내에 있을 때 전진
                Motor_Controller(1, true, 50);
                Motor_Controller(2, true, 50);
                }
                else if(x < V1){    // 왼쪽으로 회전
                    Motor_Controller(1, true, 30);
                    Motor_Controller(2, true, 50);      // pid제어를 통해 값 변화에 따라 회전값 증가 필요
                }
                else(x > V2){
                    Motor_Controller(1, true, 50);
                    Motor_Controller(2, true, 30);
                }
            }
        case 2:     // Screen interaction  나중에 없애도 됌.
            
            // sub~ screen
            break;

            /if(x < V1){    // 제자리 왼쪽으로 회전
                Motor_Controller(1, true, 30);
                Motor_Controller(2, false, 30);      // pid제어를 통해 값 변화에 따라 회전값 증가 필요
            }
            else(x > V2){   // 제자리 오른쪽 회전
                Motor_Controller(1, false, 30);
                Motor_Controller(2, true, 30);      // 반복문이라 생각 조금 더
        case 3:
            if(detect2)     // 표지판 detection
            {
                if(x_sign == V)   // 표지판 정면(V)에 두기
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
                        sequence = 0;
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
            sequence = 0;
    }
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}






