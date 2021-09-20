# Arduino Due + Simulinkでモータを回そう
## 概要
Arduino DueとX-NUCLEO-IHM07M1を使って，Simulink環境でモータをセンサレスで回します．
## 環境
1. Arduino Due
2. X-NUCLEO-IHM07M1
3. MATLAB/Simulink R2020a on Windows 10
4. Simulink Support Package for Arduino Hardware
5. MATLAB Support for MinGW-w64 C/C++ Compiler
## ピン接続
ピン接続は def_param.m に書かれています．制御器を20kHzの周期で計算するために，ハードウエア割り込みを使っています．Aruduino DueのD41をD2へ接続してください．
## 実行方法
1. def_param.mを実行し，変数を定義します．
2. motor_sensorless.slxをExternal modeで実行してください．Scopeには，目標速度と，実際の速度の推定値が表示されます．
3. VR1を右に回すとControl Onになります．
4. 次に，VR2をゆっくり右に回すと，同期モードで回り始め，2000rpmを超えるとセンサレスに切り替わります．
## コメント
1. 以前公開した，arduino_due_motor_control をSimulinkへ移植したものです．ただし，なぜか同じ性能が出ず，不安定ぎみだったので，電流制御系とPLL制御系の帯域を若干落としています．
2. センサレスは，αβ軸上の誘起をarctanするだけの簡単なものです．とりあえず動きます程度のものですので，機能追加や性能アップにチャレンジしてみてください．
3. Arduino DueのプロセッサAtmel SAM3X8EはFPUを持っていないので，Simulink上の演算は，int32の整数演算としています．
4. 開発環境ではSimulink Coderがインストールされていますが，モデル設定>ハードウエア実行>Simulink Coder機能の使用を無効化しているので，MATLAB Homeでも多分動作すると思います．
