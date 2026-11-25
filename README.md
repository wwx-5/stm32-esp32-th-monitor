# STM32 + ESP32 温湿度远程监测系统（DHT11）

## 项目简介
STM32 通过单总线读取 DHT11 温湿度，OLED 本地显示，同时经
UART 发给 ESP32；ESP32 连接 Wi-Fi 后启动 WebServer(80)，
手机访问其 IP 即可查看实时日志，页面支持一键清空。

## 系统架构
DHT11 --1-Wire--> STM32 --UART115200--> ESP32 --HTTP--> 手机
                    |
                   OLED
  采集层            预处理层            网关层        展示层

## 硬件接线
| 信号 | STM32 | 对端 |
|------|-------|------|
| DHT11 DATA | PAx（按实际） | DHT11 DATA |
| I2C SCL/SDA | PB6 / PB7 | OLED |
| UART1 TX/RX | PA9 / PA10 | ESP32 GPIO16(RX2) / 17(TX2) |

> 引脚以实际接线为准。

## 串口通信
波特率 115200，ESP32 侧使用 Serial2 (UART2, RX16/TX17)。
建议帧格式：`$T,24.0,49.0*CS\r\n`
`$` 帧头 / `T` 类型 / 逗号分隔 / `*CS` 校验 / `\r\n` 帧尾

## 开发环境
- STM32：Keil MDK + 标准外设库
- ESP32：Arduino-ESP32（WiFi.h + WebServer.h）
- 网页：HTML + JS 内联于固件（rawliteral），meta refresh 2s 轮询

## 遇到的问题与解决
1. **读数出现 `temp 0, humi 0`**
   DHT11 单总线时序敏感，单次读取失败率较高。
   修复：增加超时判断与失败重试，读取失败时不打印，
   并对数值做范围合理性校验。
2. **湿度读数漂移 / 跳变**
   原因：上电未等传感器稳定（DHT11 需 ≥1s），采样频率过高。
   修复：上电延时，采样周期设为 ≥2s（DHT11 最高 1Hz）。
3. **网页日志粘包错位**
   原因：ESP32 侧 `while(Serial2.available())` 仅以 `\n` 作为
   分界，一旦丢失字节则整屏错位。
   修复：增加帧头 `$` 与校验 `*CS`，接收端用状态机解析。

## 目录结构
