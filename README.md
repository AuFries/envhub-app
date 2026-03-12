# EnvHub (In Progress)

This is the *app* repo for the Envhub project. See the [overview wiki](https://github.com/AuFries/envhub-platform/wiki/CU-Boulder-Project-Overview) and the [project page](https://github.com/users/AuFries/projects/1) for more details.

**EnvHub** is a battery-powered embedded Linux environmental monitoring system built on the **BeagleBone Black**. It integrates multiple sensors, a touchscreen LCD UI, RTC/NTP timekeeping, safe software-controlled shutdown, and a custom Buildroot-based Linux platform.

The project was designed to demonstrate practical embedded Linux engineering across the full stack: board integration, Linux configuration, device tree work, driver bring-up, userspace services, graphics/UI, power management, and deployment workflow.


<img width="585" height="552" alt="Embedded_Linux_Monitoring_Hub(3)" src="https://github.com/user-attachments/assets/da49f285-b08a-4c75-a941-9542b526342a" />

## Features

- Custom **Buildroot** Linux platform for BeagleBone Black
- Touchscreen LCD user interface built with **LVGL**. [(UI wiki)](https://github.com/AuFries/envhub-platform/wiki/User-Interface)
- Real-time display of environmental and system measurements.
- Integrated sensors for [(Driver wiki)](https://github.com/AuFries/envhub-platform/wiki/Driver-Details):
  - CO2
  - eCO2 / TVOC
  - temperature
  - humidity
  - pressure
  - battery voltage/current/capacity
- **Battery-powered operation** with rechargeable LiPo support.
- **Soft power switch** with push-button initiated safe shutdown flow. [Shutdown wiki](https://github.com/AuFries/envhub-platform/wiki/Soft-Shutdown)
- **RTC + NTP** time synchronization. [NTP + RTC wiki](https://github.com/AuFries/envhub-platform/wiki/Time-Synchronization)
- Linux userspace services for sensor polling and UI updates. [(envhub-app wiki)](https://github.com/AuFries/envhub-platform/wiki/Envhub%E2%80%90App)
- Logging for diagnostics and system behavior. [(Logging wiki)](https://github.com/AuFries/envhub-platform/wiki/Logging)
- **TFTP + NFS** development workflow for fast iteration. [(TFTP + NFS wiki)](https://github.com/AuFries/envhub-platform/wiki/TFTP-&-NFS-Boot)

## Hardware
As the focus of this project is on the embedded linux aspect the hardware was approached with a modular perfboard layout rather than a custom PCB. A custom adapter board was soldered and plugged into the Beaglebone Black headers. See the [hardware wiki](https://github.com/AuFries/envhub-platform/wiki/Hardware) for details on the sensors used.

### Components
- BeagleBone Black
- TFT LCD with touchscreen
- Rechargeable LiPo battery
- Battery fuel-gauge / power-monitoring devices
- Environmental sensors connected over **I2C**, **SPI**, and Linux kernel subsystems such as **IIO** and **power_supply**

<img src="https://github.com/user-attachments/assets/671132fa-8d4b-49b7-a08a-96f213f0311c" alt="top_view" width="500">
<img src="https://github.com/user-attachments/assets/c1eecf54-72d1-43e1-9931-a06e368e0513" alt="side_view" width="500">

## Software Architecture

The system is structured as a small embedded Linux application stack:

- **Buildroot** generates the target root filesystem, toolchain, and kernel integration
- **Device tree overlays** configure attached peripherals and buses
- Linux kernel drivers expose sensors through standard sysfs / subsystem interfaces
- A native C application
  - sensor acquisition
  - power-state handling
  - UI updates
  - time display
  - shutdown interaction
- **LVGL** provides the graphical user interface for the LCD

### Repository Contents

[envhub-platform](https://github.com/AuFries/envhub-platform) contains buildroot, board-integration, kernel config/DT, and packaging. \
[envhub-app](https://github.com/AuFries/envhub-app) contains the userspace application for UI, sensor reading, logging, etc
