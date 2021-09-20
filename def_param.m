%% Parameter definition
%% 2021-09-17 by M.Hirata
% Inverter: X-NUCLEO-IHM07M1
% Arduino: Due
% Connection
% Arduino -> X-NUCLEO-IHM07M1 board
% D08  -> CN10 22 (LED)
% D09  -> CN10 13 (EN)
% D11  -> CN7 1 (EN1)
% D12  -> CN7 2 (EN2)
% D13  -> CN7 3 (EN3)
% D35  -> CN10 23 (IN1 PWM U)
% D37  -> CN10 21 (IN2 PWM V)
% D39  -> CN10 33 (IN3 PWM W)
% D41  -> D2 on Arduino for hardware interrupt
% A0   -> CN7 28 (Curr_fdbk1)
% A1   -> CN7 36 (Curr_fdbk2)
% A2   -> CN7 38 (Curr_fdbk3)
% A6   -> VR1 (Control ON/OFF)
% A7   -> VR2 (Speed)
% 3.3V -> CN7 16 (3.3V)
% GND  -> CN7 20 (GND)

%% clear
clear
%% Parameters
% Base sampling time for simulink block
ts = 10e-3;

VCC = 12;
IV_SCALE = 1024;
SC_SCALE = 32768;
MAX_PWM_DUTY = 1050;
FS = 20000; % sampling frequency of motor control

sqrt23 = round(sqrt(2.0/3.0)*SC_SCALE);
cos23z = round(cos(2.0/3.0*pi)*sqrt(2.0/3.0)*SC_SCALE);
cos43z = round(cos(4.0/3.0*pi)*sqrt(2.0/3.0)*SC_SCALE);
sin23z = round(sin(2.0/3.0*pi)*sqrt(2.0/3.0)*SC_SCALE);
sin43z = round(sin(4.0/3.0*pi)*sqrt(2.0/3.0)*SC_SCALE);

DUTY2VOLT = round(VCC*IV_SCALE/MAX_PWM_DUTY);
COUNT2RPM = round(FS/SC_SCALE*60/7);
adgain_and_ivscale = round(0.33*1.56*4096/3.3*SC_SCALE/IV_SCALE);
FF_CURRENT = 1500; % 同期モードのq軸電流目標値
%% Model parameters
Ra       = 1.016; % 実測値
% Ra       = 0.73+0.33+0.11; % 公称値
Ra_scale = round(Ra*SC_SCALE);
La       = 0.018e-3*(3/2);
%% PLL Controller design
omega_pll = 2*pi*50; % bandwidth
a = 2*0.7*omega_pll;
b = omega_pll^2;
Gp  = tf([a, b],[1, a, b]);
Gpz = c2d(Gp,1/FS,'zoh');
[num,den] = tfdata(Gpz);
num = num{:};
den = den{:};
az = den(2);
bz = den(3);
Kp_pll = (az+2);
Ki_pll = (az+2 - 1 + bz);
iKp_pll = round(1/Kp_pll);
iKi_pll = round(1/Ki_pll);
%% Current control design
alpha = 2*pi*1000; % bandwidth
Ki_curr = round(alpha*Ra);
Kp_curr = alpha*La;
iKp_curr = round(1/Kp_curr);
Ki_curr_d = Ki_curr;
Ki_curr_q = Ki_curr;
iKp_curr_d = iKp_curr;
iKp_curr_q = iKp_curr;
%% Velocity control gain
Kp_velo = 5;

%% EOF
