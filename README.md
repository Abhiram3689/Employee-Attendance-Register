# RFID Based Attendance System

A simple attendance system built using LPC2148 microcontroller and a Linux PC. The microcontroller reads an ID (via UART), appends a real-time timestamp from DS1307 RTC over I2C, and sends it to the PC over serial. The PC side checks the ID against a database and logs the entry as IN or OUT.

---

## How It Works

```
[RFID/Input] --> [LPC2148] --> UART --> [Linux PC]
                    |                       |
                DS1307 RTC            data_base file
                LCD Display            log file
```

1. LPC2148 waits for input over UART (UART0 interrupt driven)
2. Once data is received, it reads current time and date from DS1307 RTC over I2C
3. Appends timestamp to the received ID string and sends it to PC via UART
4. Also displays time and date on a 16x2 LCD
5. PC reads from `/dev/ttyUSB0`, parses the ID and timestamp
6. Checks ID in `data_base` file — if found, logs IN or OUT to `log` file based on previous entries

---

## Hardware Used

- LPC2148 (ARM7 microcontroller)
- DS1307 RTC module (I2C)
- 16x2 LCD
- USB to UART converter (FTDI based)
- Linux PC

---

## File Structure

```
attendance-system/
├── README.md
├── pc_side/
│   └── main.c              # Linux serial port reader + database logger
└── lpc2148_firmware/
    ├── rtc_main.c          # Main firmware - RTC read, LCD display, UART send
    ├── uart_interrupt.c    # UART0 interrupt handler + VIC config
    └── uart0driver.c       # UART0 driver (init, tx, rx, string, float, int)
```

---

## PC Side Setup

Compile:
```bash
gcc pc_side/main.c -o attendance
```

Run:
```bash
./attendance
```

Make sure `/dev/ttyUSB0` is the correct port. Check with:
```bash
dmesg | grep tty
```

You may need permission to access the port:
```bash
sudo chmod 666 /dev/ttyUSB0
```

### data_base file format
Each line has an ID followed by the person's name:
```
E001 Rahul
E002 Priya
E003 Arjun
```

### log file output
```
E001 Rahul 10:30 AM 19/04/24 in
E001 Rahul 06:15 PM 19/04/24 out
```

---

## Firmware Side

Compiled using Keil uVision for LPC2148. The initial RTC values (date, time) are hardcoded in `rtc_main.c` and can be changed before flashing:

```c
i2c_byte_write_frame(0xD0,0x4,0x16);  // date
i2c_byte_write_frame(0xD0,0x5,0x07);  // month
i2c_byte_write_frame(0xD0,0x6,0x24);  // year
i2c_byte_write_frame(0xD0,0x2,0x71);  // hours (12hr format)
i2c_byte_write_frame(0xD0,0x1,0x59);  // minutes
i2c_byte_write_frame(0xD0,0x0,0x48);  // seconds
```

---

## What I Learned

- Bare metal UART driver with interrupt handling on ARM7
- I2C communication with DS1307 RTC
- Linux serial port configuration using `termios`
- File based database lookup and logging in C
