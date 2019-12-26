# robotsystem2019

# LED２個を用いた点滅実験
・デバイスファイルに書き込まれた秒数分
　（0～9の自然数）緑、赤のLEDが交互に点滅します

# 使用環境・言語
使用環境：Raspbian

使用言語：C

# セットアップ
ラズパイのGPIO23・24ポートにLEDをそれぞれ接続
抵抗はそれぞれ300Ω程度
その後、以下のようにセットアップを行います

$make

$sudo insmod myledshingou.ko

$sudo mknod /dev/myled0 c [メジャー番号] [マイナー番号]

$sudo chmod 666 /dev/myled0

# 使い方
$ echo [秒数] > /dev/myled0

秒数は0～9の自然数


例１）
$ echo 1 > /dev/myled0 

→ GPIO23に接続されたLEDが１秒間点灯後、消灯
　その後、GPIO24に接続されたLEDが１秒間点灯後、消灯
 
 
 例２）
 $ echo 13 > /dev/myled0 

→ GPIO23に接続されたLEDが１秒間点灯後、消灯
　その後、GPIO24に接続されたLEDが１秒間点灯後、消灯
 
→ 続いて、GPIO23に接続されたLEDが３秒間点灯後、消灯
　その後、GPIO24に接続されたLEDが３秒間点灯後、消灯

# 動画
あとでのっけます
