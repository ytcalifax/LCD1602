# <div align="center">

```
                                        _     ____ ____  _  __    ___ ____  
                                       | |   / ___|  _ \/ |/ /_  / _ \___ \ 
                                       | |  | |   | | | | | '_ \| | | |__) |
                                       | |__| |___| |_| | | (_) | |_| / __/ 
                                       |_____\____|____/|_|\___/ \___/_____|
                                      
```

</div>

<p align="center">
  <a href="#">
    <img src="https://img.shields.io/badge/arduino-library-blue.svg" alt="Arduino Library Badge"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/platformio-library-orange.svg" alt="PlatformIO Badge"/>
  </a>
  <img src="https://img.shields.io/github/check-runs/ytcalifax/LCD1602/master?logo=githubactions&logoColor=%23ffffff" alt="Check Runs Badge"/>
  <a href="#">
    <img src="https://img.shields.io/github/actions/workflow/status/ytcalifax/LCD1602/docs.yml?label=docs&logo=google%20docs&logoColor=%23efefef" alt="Documentation Status Badge"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/github/commits-since/ytcalifax/LCD1602/latest?color=yellow&logo=semanticrelease" alt="GitHub Commits Badge"/>
  </a>
</p>

LCD1602 RGB is an open-source Arduino library for controlling 16x2 LCD displays with RGB backlight. It is designed for displays using the following controller ICs:

- **LCD Controller IC:** AiP31068L or Equivalent
- **RGB Controller IC:** PCA9633DP2 or Equivalent

With this library, you can easily display text and control the RGB backlight color on your LCD module. The API is simple and flexible, making it easy to integrate into your Arduino projects.

## Quick Start

LCD1602 RGB makes it easy to get started with RGB LCD modules. The library supports a wide range of Arduino boards and is compatible with both the Arduino IDE and PlatformIO.

### Installation

#### Arduino IDE (Library Manager)
1. Open the Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for `LCD1602_RGB` and click **Install**.

#### PlatformIO
1. Open PlatformIO IDE or VSCode with PlatformIO extension.
2. Add the following to your `platformio.ini`:
   ```ini
   lib_deps =
       ytcalifax/LCD1602_RGB
   ```
3. Save and build your project.

## Usage

Here is a simple example to get you started:

```cpp
#include <Wire.h>
#include <LCD1602_RGB.h>

Waveshare_LCD1602_RGB lcd(16,2);  // 16 characters and 2 lines
int r, g, b, t = 0;

void setup() {
    // initialize
    lcd.init();
    lcd.setCursor(0,0);
    lcd.send_string("Waveshare");
    lcd.setCursor(0,1);
    lcd.send_string("Hello,World!");
}

void loop() {
    r = (abs(sin(3.14*t/180)))*255;
    g = (abs(sin(3.14*(t + 60)/180)))*255;
    b = (abs(sin(3.14*(t + 120)/180)))*255;
    t = t + 3;
    lcd.setRGB(r,g,b);
    delay(150);
}
```

This example displays text on the LCD and cycles the RGB backlight through a smooth color transition.

## File Structure
- `LCD1602_RGB.cpp` - LCD and RGB control implementation
- `LCD1602_RGB.h`   - Header file for the display class
- `README.md`       - Project documentation

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Have a question or need help? Open an issue or start a discussion!**
